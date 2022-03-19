from PySide6 import QtCore, QtWidgets

from .ui_TlkChooserWidget import Ui_TlkFiles


class TlkChooserWidget(QtWidgets.QDialog, Ui_TlkFiles):
    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.setupUi(self)

        self.fileDefaultTlk.clicked.connect(self.setDefaultTlk)
        self.fileCustomTlk.clicked.connect(self.setCustomTlk)

    def setDefaultTlk(self):
        fileName, _ = QtWidgets.QFileDialog.getOpenFileName(
            self, "Open TLK File", "", "TLK Files (*.tlk)"
        )
        if fileName:
            self.defaultTlk.setText(fileName)

    def setCustomTlk(self):
        fileName, _ = QtWidgets.QFileDialog.getOpenFileName(
            self, "Open TLK File", "", "TLK Files (*.tlk)"
        )
        if fileName:
            self.defaultTlk.setText(fileName)

    def accept(self):
        print("accepted")
        pass

    def reject(self):
        print("rejected")
        self.hide()
