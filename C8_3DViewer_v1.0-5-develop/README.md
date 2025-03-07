# 3DViewer

![C](https://img.shields.io/badge/C-11-blue.svg) ![Лицензия](https://img.shields.io/badge/Лицензия-MIT-green.svg) ![Статус](https://img.shields.io/badge/Статус-В_разработке-yellow.svg)

Программа `3DViewer` — это приложение для визуализации каркасных 3D-моделей в трёхмерном пространстве. Разрабатывается на языке C стандарта C11 с использованием компилятора GCC и библиотеки Qt для графического интерфейса. Проект направлен на создание удобного инструмента для загрузки, преобразования и настройки отображения моделей формата `.obj`.

## Содержание

- [Описание](#описание)
- [Требования](#требования)
- [Установка](#установка)
- [Использование](#использование)
- [Функциональность](#функциональность)
- [Тестирование](#тестирование)
- [Покрытие кода](#покрытие-кода)
- [Как внести вклад](#как-внести-вклад)
- [Лицензия](#лицензия)

## Описание

`3DViewer` позволяет пользователям загружать каркасные модели из файлов формата `.obj`, выполнять аффинные преобразования (перемещение, поворот, масштабирование) и настраивать визуализацию (тип проекции, стиль ребер и вершин, цвет фона). Программа разрабатывается в соответствии с принципами структурного программирования и Google Style, с акцентом на производительность и удобство интерфейса.

Код проекта размещён в папке `src`, а сборка настроена через `Makefile` с стандартными целями для GNU-программ.

## Требования

- **Язык**: C11
- **Компилятор**: GCC
- **GUI-библиотека**: Qt (возможны альтернативы: GTK+, CEF и др.)
- **Платформы**: Linux, macOS
- **Производительность**: Поддержка моделей до 1 000 000 вершин без зависаний (> 0.5 сек)
- **Ограничения**: Не загружать файлы > 10 МБ в репозиторий

### Зависимости

- GCC
- Make
- Qt (для GUI)
- Библиотеки для тестирования (например, Check)
- lcov (для отчётов покрытия, опционально)

Установка на Linux:
```bash
sudo apt-get install gcc make qt5-default libcheck-dev lcov
```

Установка на macOS:
```bash
brew install gcc make qt check lcov
```

## Установка

1. Склонируйте репозиторий:
   ```bash
   git clone https://github.com/yourusername/3DViewer.git
   cd 3DViewer
   ```

2. Соберите и установите программу:
   ```bash
   make all
   make install
   ```

3. (Опционально) Удалите установленные файлы:
   ```bash
   make uninstall
   ```

4. Очистите временные файлы:
   ```bash
   make clean
   ```

Программа будет установлена в произвольный каталог (указывается в `Makefile`).

## Использование

После установки запустите программу:
```bash
./3DViewer
```

1. Нажмите кнопку "Выбрать файл" и загрузите модель `.obj`.
2. Используйте поля ввода и кнопки для перемещения, поворота или масштабирования модели.
3. Настройте визуализацию через меню настроек.
4. Сохраните изображение или GIF-анимацию (при наличии дополнительных функций).

## Функциональность

### Основные возможности (Part 1)
- Загрузка каркасных моделей из файлов `.obj` (вершины и поверхности).
- Перемещение модели по осям X, Y, Z.
- Поворот модели вокруг осей X, Y, Z.
- Масштабирование модели.
- Графический интерфейс с:
  - Кнопкой выбора файла и отображением его имени.
  - Зоной визуализации модели.
  - Полями и кнопками для управления преобразованиями.
  - Информацией о модели (имя файла, количество вершин и рёбер).

### Дополнительные настройки (Part 2)
- Выбор типа проекции: параллельная или центральная.
- Настройка рёбер: тип (сплошная/пунктирная), цвет, толщина.
- Настройка вершин: способ отображения (нет/круг/квадрат), цвет, размер.
- Выбор цвета фона.
- Сохранение настроек между запусками.

### Дополнительная запись (Part 3)
- Сохранение изображений в форматах `.bmp` и `.jpeg`.
- Запись GIF-анимаций (640x480, 10 FPS, 5 секунд) с аффинными преобразованиями.

## Тестирование

Модули загрузки моделей и аффинных преобразований должны быть покрыты юнит-тестами. Для запуска тестов:
```bash
make tests
```

Тесты проверяют:
- Корректность загрузки `.obj`-файлов.
- Точность перемещения, поворота и масштабирования.
- Обработку граничных случаев (например, пустые файлы или некорректные данные).

## Покрытие кода

Для анализа покрытия кода тестами:
```bash
make gcov_report
```

Отчёт будет сгенерирован в папке `report/` в формате HTML. Откройте `report/index.html` для просмотра.

## Как внести вклад

Приветствуются любые улучшения! Чтобы помочь проекту:

1. Сделайте форк репозитория.
2. Создайте новую ветку (`git checkout -b feature/ваше-улучшение`).
3. Внесите изменения и закоммитьте их (`git commit -m "Добавлена новая функция"`).
4. Отправьте изменения в свой форк (`git push origin feature/ваше-улучшение`).
5. Откройте Pull Request.

Следуйте Google Style и убедитесь, что код проходит тесты (`make tests`).

### Полезные команды
- Создание документации: `make dvi`
- Упаковка проекта в архив: `make dist`
- Проверка покрытия: `make gcov_report`

## Лицензия

Проект распространяется под лицензией MIT. Подробности см. в файле [LICENSE](LICENSE).

---
