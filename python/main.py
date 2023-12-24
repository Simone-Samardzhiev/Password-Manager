import json
import sys
from random import randint
from functools import partial

from PyQt6.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QLineEdit, QGridLayout, QMessageBox
from PyQt6.QtGui import QIntValidator, QCloseEvent


class Data:
    data: list[dict]

    def __init__(self) -> None:
        self.read_data()

    def read_data(self) -> None:
        with open("/Users/simonesamardzhiev/Desktop/My projects/Password Manager/python/data.json", "r") as file:
            self.data = json.load(file)

    def write_data(self) -> None:
        with open("/Users/simonesamardzhiev/Desktop/My projects/Password Manager/python/data.json", "w") as file:
            json.dump(self.data, file, indent=4)

    def reindex(self) -> None:
        for i in range(len(self.data)):
            self.data[i]['id'] = i

    def delete_app(self, _id: int) -> None:
        self.data = [app for app in self.data if app['id'] != _id]

    def add_app(self, name: str, email: str, password: str) -> None:
        app = {
            "name": name,
            "email": email,
            "password": password,
            "id": self.get_id()
        }
        self.data.append(app)

    def get_id(self) -> int:
        try:
            return self.data[-1]['id']
        except IndexError:
            return 0

    def __iter__(self) -> dict:
        for app in self.data:
            yield app


class AddPasswordWindow(QWidget):
    data: Data
    parent: "Window"
    entryLength: QLineEdit
    entryResultPassword: QLineEdit
    entryName: QLineEdit
    entryEmail: QLineEdit
    entryPassword: QLineEdit

    def __init__(self, data: Data, parent: "Window") -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle("New application")
        self.setGeometry(300, 300, 975, 550)

        # using the arguments
        self.data = data
        self.parent = parent

        # creating widgets
        layout = QGridLayout()
        self.entryLength = QLineEdit()
        self.entryResultPassword = QLineEdit()
        button_create_password = QPushButton("Create password")
        self.entryName = QLineEdit()
        self.entryEmail = QLineEdit()
        self.entryPassword = QLineEdit()
        button_add_app = QPushButton("Add application")

        # setting attributes to the widgets
        self.entryLength.setFixedSize(25, 23)
        self.entryLength.setValidator(QIntValidator(1, 99))
        button_create_password.clicked.connect(self.on_create_password_clicked)
        button_add_app.clicked.connect(self.on_add_app_clicked)

        # adding the widgets
        layout.addWidget(QLabel("Enter the length :"), 0, 0)
        layout.addWidget(self.entryLength, 0, 1)
        layout.addWidget(QLabel("Generated password :"), 1, 0)
        layout.addWidget(self.entryResultPassword, 1, 1)
        layout.addWidget(button_create_password, 2, 0)
        layout.addWidget(QLabel("Enter the name :"), 3, 0)
        layout.addWidget(self.entryName, 3, 1)
        layout.addWidget(QLabel("Enter the email :"), 4, 0)
        layout.addWidget(self.entryEmail, 4, 1)
        layout.addWidget(QLabel("Enter the password :"), 5, 0)
        layout.addWidget(self.entryPassword, 5, 1)
        layout.addWidget(button_add_app, 6, 0)

        self.setLayout(layout)

    def on_create_password_clicked(self) -> None:
        password = ''
        try:
            length = int(self.entryLength.text())
        except ValueError:
            QMessageBox.warning(self, "Error", "Please enter valid length")
            return

        letters_lowercase = ('q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
                             'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z',
                             'x', 'c', 'v', 'b', 'n', 'm')

        letters_uppercase = ('Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
                             'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z',
                             'X', 'C', 'V', 'B', 'N', 'M')

        symbols = ('_', '-', '!', '.', '@', '#', '$', '%', '^', '&',
                   '*', '(', ')', '[', ']', '{', '}', '|', '\\', '/',
                   ':', ';', '<', '>', '?', '~')

        digits = ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')

        while length != 0:
            match randint(0, 3):
                case 0:
                    password += letters_lowercase[randint(0, 25)]
                case 1:
                    password += letters_uppercase[randint(0, 25)]
                case 2:
                    password += symbols[randint(0, 25)]
                case 3:
                    password += digits[randint(0, 9)]

            length -= 1

        self.entryResultPassword.setText(password)

    def on_add_app_clicked(self) -> None:
        name = self.entryName.text()
        email = self.entryEmail.text()
        password = self.entryPassword.text()
        self.data.add_app(name, email, password)
        self.parent.search()
        self.close()


class ResultWindow(QWidget):
    app: dict
    data: Data
    parent: "Window"

    def __init__(self, app: dict, data: Data, parent: "Window") -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle(app['name'])
        self.setGeometry(100, 100, 300, 300)

        # using the arguments
        self.app = app
        self.data = data
        self.parent = parent

        # creating the widgets
        layout = QGridLayout()
        email_label = QLabel(f"Email : {app['email']}")
        password_label = QLabel(f"Password : {app['password']}")
        button_delete = QPushButton("Delete")

        # setting attributes to the widgets
        button_delete.clicked.connect(self.on_delete_clicked)

        # adding the widgets
        layout.addWidget(email_label, 0, 0)
        layout.addWidget(password_label, 1, 0)
        layout.addWidget(button_delete, 2, 0)

        self.setLayout(layout)

    def on_delete_clicked(self) -> None:
        self.data.delete_app(self.app['id'])
        self.parent.search()
        self.close()


class Window(QWidget):
    data: Data = Data()
    layout: QGridLayout
    searchBar: QLineEdit
    results: list[QPushButton] = []
    window1: ResultWindow
    window2: AddPasswordWindow

    def __init__(self) -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle("Password Manager")
        self.setGeometry(200, 200, 600, 600)

        # creating widgets
        self.layout = QGridLayout()
        button_add = QPushButton("Add")
        self.searchBar = QLineEdit()

        # setting attributes to the widgets
        button_add.clicked.connect(self.add_clicked)
        self.searchBar.returnPressed.connect(self.search)

        # adding the widgets
        self.layout.addWidget(button_add, 0, 0)
        self.layout.addWidget(self.searchBar, 1, 0)

        self.setLayout(self.layout)

    def search(self) -> None:
        for result in self.results:
            self.layout.removeWidget(result)
        self.results.clear()

        text = self.searchBar.text()
        row = 2

        for app in self.data:
            if app['name'].startswith(text):
                result = QPushButton(app['name'])
                result.clicked.connect(partial(self.result_clicked, app))
                self.layout.addWidget(result, row, 0)
                self.results.append(result)
                row += 1

    def add_clicked(self) -> None:
        self.window2 = AddPasswordWindow(self.data, self)
        self.window2.show()

    def result_clicked(self, app: dict) -> None:
        self.window1 = ResultWindow(app, self.data, self)
        self.window1.show()

    def closeEvent(self, event: QCloseEvent) -> None:
        self.data.write_data()
        event.accept()


if __name__ == "__main__":
    application = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(application.exec())
