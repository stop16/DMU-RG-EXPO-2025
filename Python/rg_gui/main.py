import sys, os
from PySide6.QtWidgets import QApplication, QMainWindow

from RG_MainWindow import Ui_MainWindow

import gui_link


if __name__ == "__main__":

    app = QApplication(sys.argv)

    main_window_instance = QMainWindow()
    ui_logic_adapter = Ui_MainWindow()
    ui_logic_adapter.setupUi(main_window_instance)

    app_logic = gui_link.MainWindow(main_window_instance)

    ui_logic_adapter.centralwidget.updateGeometry()
    ui_logic_adapter.centralwidget.layout().activate()
    main_window_instance.show()
    sys.exit(app.exec())
