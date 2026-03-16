# Universal Convertor

Конвертер между системами счисления: позиционная (любое основание), факториальная, Фибоначчи, сбалансированная троичная.

## Зависимости

- Qt 5 или 6 (модули: core, gui, widgets)
- qmake
- make
- C++17

## Первый запуск

```bash
mkdir -p build && cd build
qmake ../UniversalConverter.pro
make
./UniversalConverter
```

## Последующие запуски

После изменения кода:
```bash
cd build && make && ./UniversalConverter
```

После изменения `.pro` файла (добавление новых файлов и т.д.):
```bash
cd build && qmake ../UniversalConverter.pro && make && ./UniversalConverter
```
