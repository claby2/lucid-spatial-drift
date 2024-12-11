#pragma once
#include <QObject>

class VariableWatcher : public QObject
{
    Q_OBJECT
public:
    explicit VariableWatcher(QObject *parent = nullptr);

    int getValue() const { return value; }

public slots:
    void setValue(int newValue) {
        if (value != newValue) {
            value = newValue;
            emit valueChanged(value);
        }
    }

signals:
    void valueChanged(int newValue);

private:
    int value;

};
