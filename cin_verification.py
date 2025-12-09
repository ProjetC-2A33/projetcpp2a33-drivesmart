import sys
import time
import os

# Check required modules
try:
    import serial
except ImportError:
    print("ERROR: pyserial module not found!")
    print("Install it with: pip install pyserial")
    input("\nPress Enter to exit...")
    sys.exit(1)

try:
    import cv2
except ImportError:
    print("ERROR: opencv-python module not found!")
    print("Install it with: pip install opencv-python")
    input("\nPress Enter to exit...")
    sys.exit(1)

try:
    import numpy as np
    import imutils
    import easyocr
    import re
except ImportError as e:
    print(f"ERROR: Missing module - {e}")
    print("Install all requirements with:")
    print("pip install pyserial opencv-python easyocr numpy imutils")
    input("\nPress Enter to exit...")
    sys.exit(1)

print("All modules loaded successfully!")
print("="*50)

def capture_image_from_camera():
    """Capture image from webcam"""
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Camera not accessible.")
        return None

    # Wait for camera to warm up
    time.sleep(1)
    
    ret, frame = cap.read()
    cap.release()

    if ret:
        # Save captured image for debugging
        cv2.imwrite('captured_cin.jpg', frame)
        print("Image captured successfully")
        return frame
    else:
        print("Error: Failed to capture image.")
        return None

def process_cin_image(image):
    """Process image to extract CIN number"""
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    # Apply preprocessing
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    thresh = cv2.adaptiveThreshold(blurred, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, 
                                   cv2.THRESH_BINARY, 11, 2)
    
    # Initialize OCR reader
    reader = easyocr.Reader(['en'], gpu=False)
    result = reader.readtext(thresh)
    
    # Extract all text
    all_text = []
    for detection in result:
        text = detection[1]
        all_text.append(text)
    
    print(f"Detected text: {all_text}")
    
    # Look for 8-digit CIN number
    cin_pattern = r'\b\d{8}\b'
    for text in all_text:
        # Remove spaces and special characters
        cleaned_text = re.sub(r'[^0-9]', '', text)
        if len(cleaned_text) == 8:
            print(f"Found CIN: {cleaned_text}")
            return cleaned_text
    
    # Try to find numbers and combine them
    all_numbers = ''.join(re.findall(r'\d+', ' '.join(all_text)))
    if len(all_numbers) >= 8:
        cin = all_numbers[:8]
        print(f"Extracted CIN from combined numbers: {cin}")
        return cin
    
    print("No valid CIN found")
    return None

def run_cin_verification(ser):
    """Main function to capture and verify CIN"""
    print("=== CIN Verification System ===", flush=True)
    print("Capturing image from webcam...", flush=True)
    
    image = capture_image_from_camera()
    if image is not None:
        print("Processing image to extract CIN...", flush=True)
        cin = process_cin_image(image)
        
        if cin:
            # Send CIN to Qt application via STDOUT
            output = f"CIN:{cin}"
            print(output, flush=True)  # Qt reads this!
            
            print(f"CIN {cin} sent to Qt - waiting for authorization decision...", flush=True)
            
            # Wait for Qt to write response file
            timeout = 10  # 10 seconds timeout
            start_time = time.time()
            response = None
            
            while time.time() - start_time < timeout:
                try:
                    if os.path.exists('qt_response.txt'):
                        print("Reading authorization from qt_response.txt", flush=True)
                        with open('qt_response.txt', 'r') as f:
                            response = f.read().strip()
                        # Delete response file after reading
                        try:
                            os.remove('qt_response.txt')
                        except:
                            pass
                        break
                except Exception as e:
                    print(f"Error reading response: {e}", flush=True)
                time.sleep(0.1)
            
            if response:
                print(f"Received authorization: {response}", flush=True)
                print(f"Sending {response} to Arduino", flush=True)
                ser.write((response + '\n').encode('utf-8'))
                time.sleep(0.5)
                return cin
            else:
                print("Timeout waiting for Qt response - sending DENIED", flush=True)
                ser.write("DENIED\n".encode('utf-8'))
                return None
        else:
            # No CIN found - send to Qt via stdout
            print("CIN:NOTFOUND", flush=True)
            time.sleep(2)
            try:
                if os.path.exists('qt_response.txt'):
                    os.remove('qt_response.txt')
            except:
                pass
            ser.write("DENIED\n".encode('utf-8'))
            return None
    else:
        # Camera error - send to Qt via stdout
        print("CIN:ERROR", flush=True)
        time.sleep(2)
        try:
            if os.path.exists('qt_response.txt'):
                os.remove('qt_response.txt')
        except:
            pass
        ser.write("DENIED\n".encode('utf-8'))
        return None
        ser.write("DENIED\n".encode('utf-8'))
        return None

def check_qt_response():
    """Check if Qt has written a response file"""
    response_file = "qt_response.txt"
    if os.path.exists(response_file):
        try:
            with open(response_file, 'r') as f:
                response = f.read().strip()
            os.remove(response_file)  # Delete after reading
            return response
        except:
            return None
    return None

def listen_for_qt_signal():
    """Listen for START signal from Qt application"""
    try:
        # Connect to serial port (COM5 for Arduino in your case)
        print("Connecting to serial port COM5...")
        ser = serial.Serial('COM5', 9600, timeout=1)
        time.sleep(2)  # Wait for connection to stabilize
        
        if ser.is_open:
            print("Serial port COM5 opened successfully!")
            print("Python script ready. Waiting for commands...")
    except serial.SerialException as e:
        print(f"Serial port error: {e}")
        print("\nPossible solutions:")
        print("1. Make sure Arduino is plugged in")
        print("2. Check if COM5 is the correct port in Device Manager")
        print("3. Close Arduino IDE Serial Monitor if open")
        input("\nPress Enter to exit...")
        return
    except Exception as e:
        print(f"Error opening serial port: {e}")
        input("\nPress Enter to exit...")
        return

    try:
        print("\n=== Listening for commands ===")
        while True:
            # Check for Arduino messages
            if ser.in_waiting > 0:
                data = ser.readline().decode('utf-8').strip()
                print(f"Received from Arduino: {data}")
                
                if data == "REQUEST_CIN":
                    print("CIN verification request received from Arduino!")
                    cin = run_cin_verification(ser)
                    
                    if cin:
                        print(f"CIN {cin} sent to Qt - waiting for authorization decision...")
                        
                        # Wait for Qt to respond via file (max 10 seconds)
                        for i in range(100):  # 100 * 0.1s = 10 seconds
                            qt_response = check_qt_response()
                            if qt_response:
                                print(f"Qt decision: {qt_response}")
                                # Forward to Arduino
                                ser.write((qt_response + '\\n').encode('utf-8'))
                                print(f"Sent to Arduino: {qt_response}")
                                break
                            time.sleep(0.1)
                        else:
                            print("Timeout waiting for Qt response - sending DENIED")
                            ser.write("DENIED\\n".encode('utf-8'))
                    else:
                        print("CIN verification failed - sending DENIED")
                        ser.write("DENIED\\n".encode('utf-8'))
                    
                    time.sleep(1)
                    
            time.sleep(0.1)
            
    except KeyboardInterrupt:
        print("\nScript interrupted by user (Ctrl+C)")
    except Exception as e:
        print(f"\nUnexpected error: {e}")
        import traceback
        traceback.print_exc()
    finally:
        if ser.is_open:
            ser.close()
            print("Serial connection closed")
        input("\nPress Enter to exit...")

if __name__ == "__main__":
    try:
        listen_for_qt_signal()
    except Exception as e:
        print(f"Fatal error: {e}")
        import traceback
        traceback.print_exc()
        input("\nPress Enter to exit...")
