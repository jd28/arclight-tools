# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'TlkChooserWidget.ui'
##
## Created by: Qt User Interface Compiler version 6.2.3
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QAbstractButton, QApplication, QDialog, QDialogButtonBox,
    QGridLayout, QGroupBox, QLabel, QLineEdit,
    QSizePolicy, QToolButton, QVBoxLayout, QWidget)

class Ui_TlkFiles(object):
    def setupUi(self, TlkFiles):
        if not TlkFiles.objectName():
            TlkFiles.setObjectName(u"TlkFiles")
        TlkFiles.resize(400, 160)
        self.verticalLayout = QVBoxLayout(TlkFiles)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.tlkGroupBox = QGroupBox(TlkFiles)
        self.tlkGroupBox.setObjectName(u"tlkGroupBox")
        self.gridLayout = QGridLayout(self.tlkGroupBox)
        self.gridLayout.setObjectName(u"gridLayout")
        self.defaultTlk = QLineEdit(self.tlkGroupBox)
        self.defaultTlk.setObjectName(u"defaultTlk")
        sizePolicy = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(2)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.defaultTlk.sizePolicy().hasHeightForWidth())
        self.defaultTlk.setSizePolicy(sizePolicy)

        self.gridLayout.addWidget(self.defaultTlk, 0, 1, 1, 1)

        self.customTlk = QLineEdit(self.tlkGroupBox)
        self.customTlk.setObjectName(u"customTlk")
        sizePolicy.setHeightForWidth(self.customTlk.sizePolicy().hasHeightForWidth())
        self.customTlk.setSizePolicy(sizePolicy)

        self.gridLayout.addWidget(self.customTlk, 1, 1, 1, 1)

        self.label = QLabel(self.tlkGroupBox)
        self.label.setObjectName(u"label")

        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)

        self.label_2 = QLabel(self.tlkGroupBox)
        self.label_2.setObjectName(u"label_2")

        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)

        self.fileDefaultTlk = QToolButton(self.tlkGroupBox)
        self.fileDefaultTlk.setObjectName(u"fileDefaultTlk")

        self.gridLayout.addWidget(self.fileDefaultTlk, 0, 2, 1, 1)

        self.fileCustomTlk = QToolButton(self.tlkGroupBox)
        self.fileCustomTlk.setObjectName(u"fileCustomTlk")

        self.gridLayout.addWidget(self.fileCustomTlk, 1, 2, 1, 1)


        self.verticalLayout.addWidget(self.tlkGroupBox)

        self.buttonBox = QDialogButtonBox(TlkFiles)
        self.buttonBox.setObjectName(u"buttonBox")
        self.buttonBox.setOrientation(Qt.Horizontal)
        self.buttonBox.setStandardButtons(QDialogButtonBox.Cancel|QDialogButtonBox.Ok)

        self.verticalLayout.addWidget(self.buttonBox)


        self.retranslateUi(TlkFiles)
        self.buttonBox.accepted.connect(TlkFiles.accept)
        self.buttonBox.rejected.connect(TlkFiles.reject)

        QMetaObject.connectSlotsByName(TlkFiles)
    # setupUi

    def retranslateUi(self, TlkFiles):
        TlkFiles.setWindowTitle(QCoreApplication.translate("TlkFiles", u"Dialog", None))
        self.tlkGroupBox.setTitle(QCoreApplication.translate("TlkFiles", u"TLK Files", None))
        self.label.setText(QCoreApplication.translate("TlkFiles", u"Default:", None))
        self.label_2.setText(QCoreApplication.translate("TlkFiles", u"Custom:", None))
        self.fileDefaultTlk.setText(QCoreApplication.translate("TlkFiles", u"...", None))
        self.fileCustomTlk.setText(QCoreApplication.translate("TlkFiles", u"...", None))
    # retranslateUi

