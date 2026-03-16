#ifndef WINDOW_H
#define WINDOW_H

#include "ISystem.h"

class Window : public QMainWindow {
    Q_OBJECT

public:
    explicit Window(QWidget* parent = nullptr);
    ~Window();

private slots:
    void convert();
    void onSourceChanged(int index);
    void onTargetChanged(int index);
    void validateInput();

private:
    void setupUI();
    std::unique_ptr<ISystem> createSystem(int index, unsigned long long base);

    QComboBox* sourceCombo;
    QComboBox* targetCombo;
    QLineEdit* sourceBaseInput;
    QLabel* sourceBaseLabel;
    QLineEdit* targetBaseInput;
    QLabel* targetBaseLabel;
    QPlainTextEdit* inputEdit;
    QPlainTextEdit* outputEdit;
    QPushButton* convertButton;
    QLabel* statusLabel;

    bool inputValid;
};

#endif
