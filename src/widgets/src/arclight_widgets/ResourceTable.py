import imp
from PySide6 import QtWidgets, QtCore, QtGui
from PySide6.QtWidgets import QMessageBox

import os.path


class ResourceTableView(QtWidgets.QTableView):
    needNewErf = QtCore.Signal()

    def __init__(self, parent=None):
        super(ResourceTableView, self).__init__(parent)
        self.setAcceptDrops(True)
        self.setDragDropMode(QtWidgets.QAbstractItemView.DropOnly)
        self.setAlternatingRowColors(True)
        self.setSelectionMode(QtWidgets.QAbstractItemView.ExtendedSelection)
        self.setSelectionBehavior(QtWidgets.QAbstractItemView.SelectRows)
        self.setObjectName("resourceTable")
        self.verticalHeader().setVisible(False)
        self.dropped = QtCore.Signal([int])
        self.deleteShortcut = QtGui.QShortcut(
            QtGui.QKeySequence(QtGui.QKeySequence.Delete), self
        )
        self.deleteShortcut.setAutoRepeat(False)
        self.deleteShortcut.activated.connect(self.delete)

    def checkDrop(self, event):
        # if event.mimeData().hasUrls():
        #     can_drop = False
        #     for url in event.mimeData().urls():
        #         u = str(url.toLocalFile())
        #         ext = os.path.splitext(u)[1][1:]
        #         if len(u) and ext in Extensions:
        #             can_drop = True
        #     if can_drop:
        #         event.setDropAction(QtCore.Qt.CopyAction)
        #         event.accept()
        #         return

        event.ignore()

    def dragEnterEvent(self, event):
        self.checkDrop(event)

    def dragMoveEvent(self, event):
        self.checkDrop(event)

    def dropEvent(self, event):
        # if event.mimeData().hasUrls():
        #     event.setDropAction(QtCore.Qt.CopyAction)
        #     event.accept()
        #     l = []
        #     yes_to_all = False
        #     for url in event.mimeData().urls():
        #         u = str(url.toLocalFile())
        #         ext = os.path.splitext(u)[1][1:]
        #         if len(u) and ext in Extensions:
        #             base = os.path.basename(u)
        #             if (
        #                 self.model()
        #                 and not yes_to_all
        #                 and self.model().sourceModel().erf.get_content_object(base)
        #             ):
        #                 b = QMessageBox.question(
        #                     self,
        #                     "Overwrite File?",
        #                     'Resource: "%s" was already found in the list, would you like to replace it?'
        #                     % base,
        #                     QMessageBox.Yes | QMessageBox.No | QMessageBox.YesToAll,
        #                 )
        #                 if b == QMessageBox.No:
        #                     continue

        #                 yes_to_all = b == QMessageBox.YesToAll
        #             l.append(u)

        #     if len(l):
        #         if self.model() is None:
        #             self.needNewErf.emit()

        #         self.model().sourceModel().addFiles(l)
        # else:
        event.ignore()

    def delete(self):
        if not self.model():
            return

        select = self.selectionModel().selectedRows()
        dele = []
        for s in select:
            idx = self.model().index(s.row(), 0)
            resref = idx.data()
            idx = self.model().index(s.row(), 1)
            ext = idx.data()
            fname = "%s.%s" % (resref, ext)
            dele.append(fname)
        if len(dele):
            self.model().sourceModel().deleteFiles(dele)
            self.clearSelection()
