import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Random;

class App {
    private final String name;
    private final String email;
    private final String password;
    private int id;

    public App(String name, String email, String password, int id) {
        this.name = name;
        this.email = email;
        this.password = password;
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public String getEmail() {
        return email;
    }

    public String getPassword() {
        return password;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }
}

class Data {
    private ArrayList<App> apps;

    public Data() {
        readData();
    }

    private void readData() {
        try (FileReader fileReader = new FileReader("/Users/simonesamardzhiev/Desktop/My projects/Password Manager/java/data.json")) {
            Gson gson = new Gson();
            Type type = new TypeToken<ArrayList<App>>() {
            }.getType();
            apps = gson.fromJson(fileReader, type);
        } catch (IOException e) {
            System.err.println("The file couldn't be opened in read data ");
            System.exit(1);
        }
    }

    public void writeData() {
        reindex();
        try (FileWriter fileWriter = new FileWriter("/Users/simonesamardzhiev/Desktop/My projects/Password Manager/java/data.json")) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            gson.toJson(apps, fileWriter);
        } catch (IOException e) {
            System.err.println("The file couldn't be opened in write data");
            System.exit(1);
        }
    }

    private void reindex() {
        for (int i = 0; i < apps.size(); i++) {
            apps.get(i).setId(i);
        }
    }

    public void deleteApp(int id) {
        for (App app : apps) {
            if (app.getId() == id) {
                apps.remove(app);
                break;
            }
        }
    }

    public void addApp(String name, String email, String password) {
        apps.add(new App(name, email, password, getId()));
    }

    private int getId() {
        if (apps.isEmpty()) {
            return 0;
        } else {
            return apps.getLast().getId() + 1;
        }
    }

    public ArrayList<App> getApps() {
        return apps;
    }
}

class AddAppWindow extends JFrame {
    private final Window window;
    private final Data data;

    private final TextField passwordLenghtTextField = new TextField();
    private final TextField generatePasswordTextField = new TextField();
    private final TextField nameTextField = new TextField();
    private final TextField emailTextField = new TextField();
    private final TextField passwordTextField = new TextField();

    public AddAppWindow(Window window, Data data) {
        super();

        // setting attributes to the window
        this.setTitle("New app");
        this.setSize(975, 600);
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.window = window;
        this.data = data;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        JButton generatePassword = new JButton("Generate password");
        JButton addApp = new JButton("Add app");

        // setting attributes to the widgets
        passwordLenghtTextField.setPreferredSize(new Dimension(800, 25));
        generatePasswordTextField.setPreferredSize(new Dimension(800, 25));
        nameTextField.setPreferredSize(new Dimension(800, 25));
        emailTextField.setPreferredSize(new Dimension(800, 25));
        passwordTextField.setPreferredSize(new Dimension(800, 25));
        generatePassword.addActionListener(actionEvent -> AddAppWindow.this.generatePasswordClicked());
        addApp.addActionListener(actionEvent -> AddAppWindow.this.addAppClicked());

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(new JLabel("Enter the length :"), layout);

        layout.gridx = 1;
        this.add(passwordLenghtTextField, layout);

        layout.gridx = 0;
        layout.gridy = 1;
        this.add(new JLabel("Generated password :"), layout);

        layout.gridx = 1;
        this.add(generatePasswordTextField, layout);

        layout.gridx = 0;
        layout.gridy = 2;
        this.add(generatePassword, layout);

        layout.gridy = 3;
        this.add(new JLabel("Enter the name"), layout);

        layout.gridx = 1;
        this.add(nameTextField, layout);

        layout.gridx = 0;
        layout.gridy = 4;
        this.add(new JLabel("Enter the email :"), layout);

        layout.gridx = 1;
        this.add(emailTextField, layout);

        layout.gridx = 0;
        layout.gridy = 5;

        this.add(new JLabel("Enter the password"), layout);

        layout.gridx = 1;
        this.add(passwordTextField, layout);

        layout.gridx = 0;
        layout.gridy = 6;
        this.add(addApp, layout);

        this.setVisible(true);
    }

    private void generatePasswordClicked() {
        int length;
        try {
            length = Integer.parseInt(passwordLenghtTextField.getText());

            if (length > 99) {
                JOptionPane.showMessageDialog(this, "The length can't be over 99", "Error!", JOptionPane.INFORMATION_MESSAGE);
                return;
            }
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "Please enter a valid length", "Error!", JOptionPane.INFORMATION_MESSAGE);
            return;
        }

        StringBuilder password = new StringBuilder();
        Random random = new Random();

        char[] lettersLowercase = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
                'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z',
                'x', 'c', 'v', 'b', 'n', 'm'};
        char[] lettersUppercase = {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
                'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z',
                'X', 'C', 'V', 'B', 'N', 'M'};
        char[] symbols = {'_', '-', '!', '.', '@', '#', '$', '%', '^', '&',
                '*', '(', ')', '[', ']', '{', '}', '|', '\\', '/',
                ':', ';', '<', '>', '?', '~'};
        char[] digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

        while (length != 0) {
            switch (random.nextInt(0, 4)) {
                case 0:
                    password.append(lettersLowercase[random.nextInt(lettersLowercase.length)]);
                    break;
                case 1:
                    password.append(lettersUppercase[random.nextInt(lettersUppercase.length)]);
                    break;
                case 2:
                    password.append(symbols[random.nextInt(symbols.length)]);
                    break;
                case 3:
                    password.append(digits[random.nextInt(digits.length)]);
                    break;
            }
            length--;
        }
        generatePasswordTextField.setText(password.toString());
    }

    private void addAppClicked() {
        String name = nameTextField.getText();
        String email = emailTextField.getText();
        String password = passwordTextField.getText();

        data.addApp(name, email, password);
        window.onSearch();
        this.dispose();
    }
}

class AppInfoWindow extends JFrame {
    private final Window window;
    private final App app;
    private final Data data;

    public AppInfoWindow(Window window, App app, Data data) {
        super();

        // setting attributes to the  window
        this.setTitle(app.getName());
        this.setSize(200, 200);
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.window = window;
        this.app = app;
        this.data = data;

        // creating the widget
        GridBagConstraints layout = new GridBagConstraints();
        JLabel emailLabel = new JLabel(String.format("Email : %s", app.getEmail()));
        JLabel passwordLabel = new JLabel(String.format("Password : %s", app.getPassword()));
        JButton buttonDelete = new JButton("Delete app");

        // connecting the button to the function
        buttonDelete.addActionListener(actionEvent -> AppInfoWindow.this.onDeletePressed());

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(emailLabel, layout);

        layout.gridy = 1;
        this.add(passwordLabel, layout);

        layout.gridy = 2;
        this.add(buttonDelete, layout);

        this.setVisible(true);
    }

    private void onDeletePressed() {
        data.deleteApp(app.getId());
        window.onSearch();
        this.dispose();
    }
}

class Window extends JFrame {
    private final Data data = new Data();
    private final JTextField searchBar = new JTextField();
    private final ArrayList<JButton> results = new ArrayList<>();

    public Window() {
        super();

        // setting attributes tho the window
        this.setTitle("Password Manager");
        this.setSize(600, 600);
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                Window.this.data.writeData();
                System.exit(0);
            }
        });
        this.setLayout(new GridBagLayout());

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        JButton buttonAdd = new JButton("Add new app");

        // setting attributes to the widgets
        buttonAdd.addActionListener(actionEvent -> new AddAppWindow(Window.this, Window.this.data));
        searchBar.setPreferredSize(new Dimension(200, 30));
        searchBar.addActionListener(actionEvent -> onSearch());

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(buttonAdd, layout);

        layout.gridy = 1;
        this.add(searchBar, layout);
    }


    public void onSearch() {
        for (JButton result : results) {
            this.getContentPane().remove(result);
        }
        results.clear();

        String text = searchBar.getText();
        GridBagConstraints layout = new GridBagConstraints();
        layout.gridx = 0;
        layout.gridy = 2;

        for (App app : data.getApps()) {
            if (app.getName().startsWith(text)) {
                JButton result = new JButton(app.getName());
                result.addActionListener(actionEvent -> new AppInfoWindow(Window.this, app, Window.this.data));
                this.add(result, layout);
                layout.gridy++;
                results.add(result);
            }
        }

        this.repaint();
        this.revalidate();
    }
}


public class Main {
    public static void main(String[] args) {
        Window window = new Window();
        window.setVisible(true);
    }
}