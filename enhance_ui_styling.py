#!/usr/bin/env python3
"""
UI Enhancement Script - Applies modern DriveSmart styling to UI files
Colors: #002157 (blue), #ffce00 (gold), white backgrounds
"""

import re
import sys

# Modern styling templates
GROUPBOX_STYLE = """QGroupBox {
  background: white;
  border-radius: 15px;
  border: 2px solid #dee2e6;
}"""

TABLE_STYLE = """QTableWidget {
  background-color: white;
  alternate-background-color: #f8f9fa;
  selection-background-color: #002157;
  selection-color: white;
  border: 1px solid #dee2e6;
  border-radius: 8px;
  gridline-color: #e9ecef;
}
QTableWidget::item {
  padding: 8px;
}
QTableWidget::item:hover {
  background-color: #e7f3ff;
}
QHeaderView::section {
  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #002157, stop:1 #004589);
  color: white;
  padding: 10px;
  border: none;
  font-weight: bold;
  font-size: 11px;
}
QHeaderView::section:hover {
  background: #003d7a;
}"""

LINEEDIT_STYLE = """QLineEdit {
  background-color: white;
  border-radius: 8px;
  border: 2px solid #ffce00;
  padding: 6px 10px;
}
QLineEdit:hover {
  border-color: #002157;
}
QLineEdit:focus {
  border: 2px solid #002157;
}"""

COMBOBOX_STYLE = """QComboBox {
  background-color: white;
  border-radius: 8px;
  border: 2px solid #ffce00;
  padding: 5px 10px;
}
QComboBox:hover {
  border-color: #002157;
}
QComboBox:focus {
  border: 2px solid #002157;
}"""

BUTTON_PRIMARY_STYLE = """QPushButton {
  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffce00, stop:1 #e6b800);
  color: #002157;
  border-radius: 10px;
  padding: 10px;
  font-weight: bold;
  border: 2px solid #d4a500;
}
QPushButton:hover {
  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #002157, stop:1 #004589);
  color: white;
  border: 2px solid #002157;
}
QPushButton:pressed {
  background: #001533;
  padding-top: 12px;
  padding-left: 2px;
}"""

BUTTON_SECONDARY_STYLE = """QPushButton {
  background-color: transparent;
  color: #6c757d;
  border: 2px solid #dee2e6;
  border-radius: 8px;
  padding: 8px;
}
QPushButton:hover {
  background-color: #f8f9fa;
  color: #495057;
  border-color: #adb5bd;
}
QPushButton:pressed {
  background-color: #e9ecef;
}"""

DATEEDIT_STYLE = """QDateEdit {
  background-color: white;
  border-radius: 8px;
  border: 2px solid #ffce00;
  padding: 5px 10px;
}
QDateEdit:hover {
  border-color: #002157;
}"""

TEXTAREA_STYLE = """QTextEdit {
  background-color: white;
  border-radius: 8px;
  border: 2px solid #ffce00;
  padding: 8px;
}
QTextEdit:focus {
  border: 2px solid #002157;
}"""

SPINBOX_STYLE = """QSpinBox, QDoubleSpinBox {
  background-color: white;
  border-radius: 8px;
  border: 2px solid #ffce00;
  padding: 5px 10px;
}
QSpinBox:hover, QDoubleSpinBox:hover {
  border-color: #002157;
}"""

def apply_modern_styling(ui_content, ui_name):
    """Apply modern DriveSmart styling to UI file content"""
    print(f"Enhancing {ui_name}...")
    
    # Apply GroupBox styling
    ui_content = re.sub(
        r'(<widget class="QGroupBox"[^>]*>.*?<property name="styleSheet">.*?<string notr="true">)(.*?)(</string>)',
        lambda m: m.group(1) + GROUPBOX_STYLE + m.group(3),
        ui_content,
        flags=re.DOTALL
    )
    
    # Apply TableWidget styling  
    ui_content = re.sub(
        r'(<widget class="QTableWidget"[^>]*>)',
        lambda m: m.group(1) + f'\n    <property name="styleSheet">\n     <string notr="true">{TABLE_STYLE}</string>\n    </property>',
        ui_content
    )
    
    # Apply LineEdit styling for inputs
    ui_content = re.sub(
        r'(<widget class="QLineEdit" name="(?!lineEdit_recherche)[^"]*"[^>]*>)',
        lambda m: m.group(1) + f'\n    <property name="styleSheet">\n     <string notr="true">{LINEEDIT_STYLE}</string>\n    </property>',
        ui_content
    )
    
    # Apply ComboBox styling
    ui_content = re.sub(
        r'(<widget class="QComboBox"[^>]*>)',
        lambda m: m.group(1) + f'\n    <property name="styleSheet">\n     <string notr="true">{COMBOBOX_STYLE}</string>\n    </property>',
        ui_content
    )
    
    # Apply primary button styling (Ajouter, Modifier, Confirmer buttons)
    ui_content = re.sub(
        r'(<widget class="QPushButton" name="(?:pushButton_ajouter|pushButton_modifier|pb_ajouter|pb_modifier|pb_confirmer|pushButton_confirmer)[^"]*"[^>]*>)',
        lambda m: m.group(1) + f'\n    <property name="styleSheet">\n     <string notr="true">{BUTTON_PRIMARY_STYLE}</string>\n    </property>',
        ui_content
    )
    
    # Apply secondary button styling (Supprimer, Annuler buttons)
    ui_content = re.sub(
        r'(<widget class="QPushButton" name="(?:pushButton_supprimer|pushButton_annuler|pb_supprimer|pb_annuler|pb_quitter)[^"]*"[^>]*>)',
        lambda m: m.group(1) + f'\n    <property name="styleSheet">\n     <string notr="true">{BUTTON_SECONDARY_STYLE}</string>\n    </property>',
        ui_content
    )
    
    return ui_content

def process_ui_file(filepath):
    """Process a single UI file"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Apply styling
        enhanced_content = apply_modern_styling(content, filepath)
        
        # Write back
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(enhanced_content)
        
        print(f"✓ Successfully enhanced {filepath}")
        return True
    except Exception as e:
        print(f"✗ Error processing {filepath}: {e}")
        return False

if __name__ == "__main__":
    ui_files = [
        "vehicule.ui",
        "planning.ui", 
        "pageemploye.ui",
        "examen.ui",
        "condidat.ui"
    ]
    
    print("=" * 60)
    print("DriveSmart UI Enhancement Tool")
    print("=" * 60)
    
    success_count = 0
    for ui_file in ui_files:
        if process_ui_file(ui_file):
            success_count += 1
    
    print("=" * 60)
    print(f"Enhanced {success_count}/{len(ui_files)} UI files successfully")
    print("=" * 60)
