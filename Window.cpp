#include "Window.h"
#include "PSystem.h"
#include "FactSystem.h"
#include "FibSystem.h"
#include "TerSystem.h"

Window::Window(QWidget* parent) : QMainWindow(parent), inputValid(false) {
    setupUI();
    onSourceChanged(0);
    onTargetChanged(0);
}

Window::~Window() = default;

void Window::setupUI() {
    setWindowTitle("UniversalConverter");
    resize(1200, 500);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QGroupBox* sourceGroup = new QGroupBox("Исходная система счисления", this);
    QGridLayout* sourceLayout = new QGridLayout(sourceGroup);

    sourceLayout->addWidget(new QLabel("Система счисления:", this), 0, 0);
    sourceCombo = new QComboBox(this);
    sourceCombo->addItem("P-ичная");
    sourceCombo->addItem("Факториальная");
    sourceCombo->addItem("Фиббоначиева");
    sourceCombo->addItem("Уравновешенная троичная");
    sourceLayout->addWidget(sourceCombo, 0, 1);

    sourceBaseLabel = new QLabel("Основание:", this);
    sourceLayout->addWidget(sourceBaseLabel, 1, 0);
    sourceBaseInput = new QLineEdit("10", this);
    sourceBaseInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{1,19}"), this));
    sourceLayout->addWidget(sourceBaseInput, 1, 1);

    sourceLayout->addWidget(new QLabel("Число:", this), 2, 0, Qt::AlignTop);
    inputEdit = new QPlainTextEdit(this);
    inputEdit->setPlaceholderText("Введите число");
    inputEdit->setMaximumBlockCount(1000);
    sourceLayout->addWidget(inputEdit, 2, 1);

    mainLayout->addWidget(sourceGroup);

    QGroupBox* targetGroup = new QGroupBox("Необходимая система счисления", this);
    QGridLayout* targetLayout = new QGridLayout(targetGroup);

    targetLayout->addWidget(new QLabel("Система счисления:", this), 0, 0);
    targetCombo = new QComboBox(this);
    targetCombo->addItem("P-ичная");
    targetCombo->addItem("Факториальная");
    targetCombo->addItem("Фиббоначиева");
    targetCombo->addItem("Уравновешенная троичная");

    targetLayout->addWidget(targetCombo, 0, 1);

    targetBaseLabel = new QLabel("Основание:", this);
    targetLayout->addWidget(targetBaseLabel, 1, 0);
    targetBaseInput = new QLineEdit("10", this);
    targetBaseInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{1,19}"), this));
    targetLayout->addWidget(targetBaseInput, 1, 1);

    targetLayout->addWidget(new QLabel("Результат:", this), 2, 0, Qt::AlignTop);
    outputEdit = new QPlainTextEdit(this);
    outputEdit->setReadOnly(true);
    outputEdit->setPlaceholderText("Место для ответа");
    targetLayout->addWidget(outputEdit, 2, 1);

    mainLayout->addWidget(targetGroup);

    QHBoxLayout* bottomLayout = new QHBoxLayout();

    statusLabel = new QLabel("Ждем ваши числа", this);
    bottomLayout->addWidget(statusLabel);
    bottomLayout->addStretch();

    convertButton = new QPushButton("Перевести", this);
    convertButton->setMinimumWidth(120);
    convertButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 10px; }"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }"
        "QPushButton:disabled { background-color: #cccccc; }"
        );
    bottomLayout->addWidget(convertButton);

    mainLayout->addLayout(bottomLayout);

    connect(convertButton, &QPushButton::clicked, this, &Window::convert);
    connect(sourceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Window::onSourceChanged);
    connect(targetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Window::onTargetChanged);
    connect(inputEdit, &QPlainTextEdit::textChanged, this, &Window::validateInput);
    connect(sourceBaseInput, &QLineEdit::textChanged, this, &Window::validateInput);
    connect(targetBaseInput, &QLineEdit::textChanged, this, &Window::validateInput);
}

void Window::onSourceChanged(int index) {
    bool isPary = (index == 0);
    sourceBaseLabel->setVisible(isPary);
    sourceBaseInput->setVisible(isPary);
}

void Window::onTargetChanged(int index) {
    bool isPary = (index == 0);
    targetBaseLabel->setVisible(isPary);
    targetBaseInput->setVisible(isPary);
}

void Window::validateInput() {
    int index = sourceCombo->currentIndex();
    QString text = inputEdit->toPlainText().trimmed();

    if (text.isEmpty()) {
        statusLabel->setText("Ждем ваши числа");
        statusLabel->setStyleSheet("");
        inputValid = false;
        return;
    }

    try {
        unsigned long long base = 10;
        if (index == 0) {
            bool ok;
            base = sourceBaseInput->text().toULongLong(&ok);
            if (!ok || base < 2 || base > BASE) {
                throw std::invalid_argument("Invalid base");
            }
        }

        auto system = createSystem(index, base);
        if (system->isValid(text.toStdString())) {
            statusLabel->setText("Ввод корректен");
            statusLabel->setStyleSheet("color: green;");
            inputValid = true;
        } else {
            throw std::invalid_argument("Ваше число не похоже на число :(");
        }
    } catch (std::invalid_argument& e) {
        statusLabel->setText(QString("ОШИБКА: %1").arg(e.what()));
        statusLabel->setStyleSheet("color: red;");
        inputValid = false;
    } catch (std::underflow_error& e) {
        statusLabel->setText(QString("ОШИБКА: %1").arg(e.what()));
        statusLabel->setStyleSheet("color: red;");
        inputValid = false;
    } catch (std::out_of_range& e) {
        statusLabel->setText(QString("ОШИБКА: %1").arg(e.what()));
        statusLabel->setStyleSheet("color: red;");
        inputValid = false;
    }
}

std::unique_ptr<ISystem> Window::createSystem(int index, unsigned long long base) {
    switch (index) {
    case 0: return std::make_unique<PSystem>(base);
    case 1: return std::make_unique<FactSystem>();
    case 2: return std::make_unique<FibSystem>();
    case 3: return std::make_unique<TerSystem>();
    default: throw std::invalid_argument("Invalid system");
    }
}

void Window::convert() {
    if (!inputValid) {
        QMessageBox::warning(this, "ОШИБКА", "Введите корректное число");
        return;
    }

    setEnabled(false);
    statusLabel->setText("Считаем....");
    statusLabel->setStyleSheet("color: blue;");
    repaint();

    try {
        int sourceIndex = sourceCombo->currentIndex();
        int targetIndex = targetCombo->currentIndex();

        unsigned long long sourceBase = 10;
        if (sourceIndex == 0) {
            bool ok;
            sourceBase = sourceBaseInput->text().toULongLong(&ok);
            if (!ok || sourceBase < 2 || sourceBase > BASE) {
                throw std::invalid_argument("Некорректное основание исходной системы");
            }
        }

        unsigned long long targetBase = 10;
        if (targetIndex == 0) {
            bool ok;
            targetBase = targetBaseInput->text().toULongLong(&ok);
            if (!ok || targetBase < 2 || targetBase > BASE) {
                throw std::invalid_argument("Некорректное основание целевой системы");
            }
        }

        auto sourceSystem = createSystem(sourceIndex, sourceBase);
        auto targetSystem = createSystem(targetIndex, targetBase);

        QString text = inputEdit->toPlainText().trimmed();

        BigInt value = sourceSystem->fromString(text.toStdString());
        std::string result = targetSystem->toString(value);

        outputEdit->setPlainText(QString::fromStdString(result));
        statusLabel->setText("Я посчитал, проверяйте :)");
        statusLabel->setStyleSheet("color: green;");

    } catch (std::exception& e) {
        QMessageBox::critical(this, "ОШИБКА", QString("ОШИБКА: %1").arg(e.what()));
        statusLabel->setText("ОШИБКА");
        statusLabel->setStyleSheet("color: red;");
    }

    setEnabled(true);
}
