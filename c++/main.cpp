#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QIntValidator>
#include <QLineEdit>
#include <QMessageBox>

#include <vector>
#include <random>

#include <QFile>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>

class App {
private:
    QString name;
    QString email;
    QString password;
    int id;
public:
    App(const QString &name, const QString &email, const QString &password, int id) {
        this->name = name;
        this->email = email;
        this->password = password;
        this->id = id;
    }

    const QString &getName() const {
        return name;
    }

    const QString &getEmail() const {
        return email;
    }

    const QString &getPassword() const {
        return password;
    }

    int getId() const {
        return id;
    }

    void setId(int _id) {
        id = _id;
    }
};

class Data {
private:
    std::vector<App> apps;
public:
    Data() {
        readData();
    }

    void writeData() {
        reindex();
        QJsonArray jsonArray;

        for (const App &app: apps) {
            QJsonObject object;
            object["name"] = app.getName();
            object["email"] = app.getEmail();
            object["password"] = app.getPassword();
            object["id"] = app.getId();
            jsonArray.append(object);
        }

        QJsonDocument jsonDocument(jsonArray);
        QByteArray byteArray = jsonDocument.toJson();

        QFile file("/Users/simonesamardzhiev/Desktop/My projects/Password Manager/c++/data.json");
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            qDebug() << "The file couldn't be opened in write data";
            return;
        }
        file.write(byteArray);
        file.close();
    }

    void deleteApp(int id) {
        for (int i = 0; i < apps.size(); i++) {
            if (apps[i].getId() == id) {
                apps.erase(apps.begin() + i);
                break;
            }
        }
    }

    void addApp(const QString &name, const QString &email, const QString &password) {
        apps.emplace_back(name, email, password, getNewId());
    }

    const std::vector<App> getApps() const {
        return apps;
    }

private:
    void readData() {
        QFile file("/Users/simonesamardzhiev/Desktop/My projects/Password Manager/c++/data.json");
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "The file couldn't be opened to read data";
            return;
        }
        QByteArray byteArray = file.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
        QJsonArray jsonArray = jsonDocument.array();

        for (const QJsonValueRef &value: jsonArray) {
            QJsonObject object = value.toObject();
            QString name = object.value("name").toString();
            QString email = object.value("email").toString();
            QString password = object.value("password").toString();
            int id = object.value("id").toInt();
            apps.emplace_back(name, email, password, id);
        }
    }

    void reindex() {
        for (int i = 0; i < apps.size(); i++) {
            apps[i].setId(i);
        }
    }

    int getNewId() {
        if (apps.empty()) {
            return 0;
        } else {
            return apps.back().getId() + 1;
        }
    }

};

class AddPasswordWindow : public QWidget {
Q_OBJECT

private:
    QLineEdit *generatedPasswordEntry;
    QLineEdit *sizeOfThePassword;
    QLineEdit *nameEntry;
    QLineEdit *emailEntry;
    QLineEdit *passwordEntry;
public:
    explicit AddPasswordWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // setting the attributes to teh window
        this->setWindowTitle("New App");
        this->setGeometry(100, 100, 975, 550);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto *layout = new QGridLayout;
        auto *buttonGenerate = new QPushButton("Generate password");
        sizeOfThePassword = new QLineEdit;
        generatedPasswordEntry = new QLineEdit;
        nameEntry = new QLineEdit;
        emailEntry = new QLineEdit;
        passwordEntry = new QLineEdit;
        auto *buttonAdd = new QPushButton("Add application");

        // setting attributes to widgets
        sizeOfThePassword->setValidator(new QIntValidator(1, 99));
        sizeOfThePassword->setFixedSize(30, 23);
        connect(buttonGenerate, &QPushButton::clicked, [=]() { generatePasswordClicked(); });
        connect(buttonAdd, &QPushButton::clicked, [=]() { addPasswordClicked(); });

        // adding the widgets
        layout->addWidget(new QLabel("Enter the length :"), 0, 0);
        layout->addWidget(sizeOfThePassword, 0, 1);
        layout->addWidget(new QLabel("Generated password :"), 1, 0);
        layout->addWidget(generatedPasswordEntry, 1, 1);
        layout->addWidget(buttonGenerate, 2, 0);
        layout->addWidget(new QLabel("Enter the name :"), 3, 0);
        layout->addWidget(nameEntry, 3, 1);
        layout->addWidget(new QLabel("Enter the email :"), 4, 0);
        layout->addWidget(emailEntry, 4, 1);
        layout->addWidget(new QLabel("Enter the password :"), 5, 0);
        layout->addWidget(passwordEntry, 5, 1);
        layout->addWidget(buttonAdd, 6, 0);

        this->setLayout(layout);
    }

signals:

    void addPassword(const QString &name, const QString &email, const QString &password);

private slots:


    void addPasswordClicked() {
        QString name = nameEntry->text();
        QString email = emailEntry->text();
        QString password = passwordEntry->text();

        emit addPassword(name, email, password);
        this->close();
    }

    void generatePasswordClicked() {
        QString password;
        int length = sizeOfThePassword->text().toInt();
        if (length == 0) {
            QMessageBox::warning(this, "Error", "Please enter a valid password length.");
            return;
        }
        char lettersLowercase[] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
                                   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z',
                                   'x', 'c', 'v', 'b', 'n', 'm'};
        char lettersUppercase[] = {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
                                   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z',
                                   'X', 'C', 'V', 'B', 'N', 'M'};
        char symbols[] = {'_', '-', '!', '.', '@', '#', '$', '%', '^', '&',
                          '*', '(', ')', '[', ']', '{', '}', '|', '\\', '/',
                          ':', ';', '<', '>', '?', '~'};
        char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());
        std::uniform_int_distribution get0to25(0, 25);
        std::uniform_int_distribution get0to9(0, 9);
        std::uniform_int_distribution get0to3(0, 3);

        while (length) {
            switch (get0to3(generator)) {
                case 0:
                    password.append(lettersLowercase[get0to25(generator)]);
                    break;
                case 1:
                    password.append(lettersUppercase[get0to25(generator)]);
                    break;
                case 2:
                    password.append(symbols[get0to25(generator)]);
                    break;
                case 3:
                    password.append(digits[get0to9(generator)]);
                    break;
            }
            length--;
        }
        generatedPasswordEntry->setText(password);
    }
};

class ResultWindow : public QWidget {
Q_OBJECT

public:
    explicit ResultWindow(const App &app, QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle(app.getName());
        this->setGeometry(150, 150, 200, 200);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto *layout = new QGridLayout;
        auto *labelEmail = new QLabel("Email : " + app.getEmail());
        auto *labelPassword = new QLabel("Password : " + app.getPassword());
        auto *deleteButton = new QPushButton("delete");

        // connect the button the signal
        connect(deleteButton, &QPushButton::clicked, [=]() { onDeleteClicked(app.getId()); });

        // adding the widgets
        layout->addWidget(labelEmail, 0, 0);
        layout->addWidget(labelPassword, 1, 0);
        layout->addWidget(deleteButton, 2, 0);

        this->setLayout(layout);
    }

signals:

    void deleteClicked(int id);

private slots:

    void onDeleteClicked(int id) {
        emit deleteClicked(id);
    }
};

class Window : public QWidget {
Q_OBJECT

private:
    Data data;
    QGridLayout *layout;
    std::vector<QPushButton *> results;
    QLineEdit *searchBar;

public:
    explicit Window(QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("Password manager");
        this->setGeometry(100, 100, 600, 600);

        // creating the widgets
        layout = new QGridLayout;
        auto *button = new QPushButton("Add app");
        searchBar = new QLineEdit;

        // connecting methods to widgets
        connect(button, &QPushButton::clicked, [=]() { onAddClicked(); });
        connect(searchBar, &QLineEdit::returnPressed, [=]() { onEnterPressed(); });

        // adding the widgets
        layout->addWidget(button, 0, 0);
        layout->addWidget(searchBar, 1, 0);

        this->setLayout(layout);
    }

private slots:

    void onAddClicked() {
        auto *addPasswordWindow = new AddPasswordWindow();
        connect(addPasswordWindow, &AddPasswordWindow::addPassword, this, &Window::addApp);
        addPasswordWindow->show();
    }

    void addApp(const QString &name, const QString &email, const QString &password) {
        data.addApp(name, email, password);
        onEnterPressed();
    }

    // method that will display the found results as buttons
    void onEnterPressed() {
        for (QPushButton *result: results) {
            layout->removeWidget(result);
            delete result;
        }
        results.clear();

        int row = 2;
        QString searchText = searchBar->text();

        for (const App &app: data.getApps()) {
            if (app.getName().startsWith(searchText)) {
                auto *result = new QPushButton(app.getName());
                connect(result, &QPushButton::clicked, [=]() { resultClicked(app); });
                layout->addWidget(result, row++, 0);
                results.push_back(result);
            }
        }
    }

    void resultClicked(const App &app) {
        auto *resultWindow = new ResultWindow(app);
        connect(resultWindow, &ResultWindow::deleteClicked, this, &Window::deleteClicked);
        resultWindow->show();
    }

    void deleteClicked(int id) {
        data.deleteApp(id);
        onEnterPressed();
    }

    void closeEvent(QCloseEvent *event) override {
        data.writeData();
        QWidget::closeEvent(event);
    }
};

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    Window window;
    window.show();
    return QApplication::exec();
}

#include "main.moc"
