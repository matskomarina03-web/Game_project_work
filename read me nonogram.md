Завдання 1. Індивідуальний проєкт (ПР23)
1. MVP та ключовий сценарій
        Постановка задачі (узагальнено ПР21)

        Застосунок дозволяє користувачу створювати, переглядати, редагувати та видаляти записи (об’єкти предметної області), а також зберігати їх між запусками програми.

        Екрани (ПР22)
        Dashboard (список записів)
        Create/Edit (створення та редагування запису)
        Settings (налаштування)
        Ключовий сценарій MVP
        Користувач відкриває Dashboard
        Створює новий запис
        Заповнює форму
        Зберігає запис
        Запис з’являється у списку
        Може редагувати або видаляти його
2. Основні сутності
2.1 Task (основна сутність MVP)

        Призначення: зберігання основного запису застосунку

        MVP: так

        Поля:

        id — int — обов’язкове — унікальний ідентифікатор
        title — string — обов’язкове — назва запису
        description — string — необов’язкове — опис
        status — string — обов’язкове — стан (new / in_progress / done)
        createdAt — datetime — обов’язкове — дата створення
        updatedAt — datetime — необов’язкове — дата оновлення
        2.2 AppSettings (налаштування застосунку)

        Призначення: зберігання параметрів інтерфейсу

        MVP: частково

        Поля:

        theme — string — обов’язкове — світла/темна тема
        lastOpenedScreen — string — необов’язкове — останній екран
        sortMode — string — необов’язкове — режим сортування
        Позам MVP:
        Categories
        Tags
        User profiles
3. Поля сутностей (деталізація)
        Task
        id (int, required)
        title (string, required)
        description (string, optional)
        status (enum string, required)
        createdAt (datetime, required)
        updatedAt (datetime, optional)
        AppSettings
        theme (string, required)
        lastOpenedScreen (string, optional)
        sortMode (string, optional)
4. Ідентифікація об’єктів
        Тип ID: числовий auto-increment
        Генерація: nextId = max(id) + 1
        Сутності з ID:
        Task → має власний id
        AppSettings → не потребує id (singleton)
5. Зв’язки між сутностями
        Task → AppSettings: немає прямого зв’язку
        MVP не використовує складні зв’язки

        Причина: спрощення структури для першої версії

6. Модель стану застосунку
        Основні дані (зберігаються)
        список Task
        AppSettings
        Тимчасовий стан (session state)
        selectedTaskId
        currentScreen
        isEditing
        hasUnsavedChanges
        Налаштування
        theme
        sortMode
        lastOpenedScreen
7. Стан екранів
        Dashboard
        selectedItemId
        emptyState (немає задач)
        normalState (є задачі)
        loadingState (завантаження)
        Create/Edit
        editingMode (create / edit)
        isDirty (є зміни)
        validationErrorState
        Settings
        changedState
        savedState
8. Спосіб збереження
        Обраний формат: JSON
        Причини:
        підтримка вкладених структур
        легко працює з Qt (QJson)
        підходить для MVP
        зручно масштабувати
        Зберігаються дані:
        список Task
        AppSettings
9. Структура JSON
        {
        "tasks": [
        {
        "id": 1,
        "title": "Example",
        "description": "Text",
        "status": "new",
        "createdAt": "2026-06-05T10:00:00",
        "updatedAt": null
        }
        ],
        "settings": {
        "theme": "light",
        "lastOpenedScreen": "dashboard",
        "sortMode": "date"
        },
        "nextId": 2
        }
10. Валідація даних
        Task:
        title ≠ empty
        status ∈ {new, in_progress, done}
        createdAt обов’язкове
        id унікальний
        AppSettings:
        theme ∈ {light, dark}
        sortMode ∈ {date, name}
11. Збереження і завантаження
        Коли завантажується:
        при старті застосунку
        Коли зберігається:
        після create/edit/delete
        при зміні налаштувань
        Якщо файл відсутній:
        створюється порожня модель
        Якщо помилка:
        показ повідомлення
        використовується дефолтний стан
12. Шар даних (Data Layer)
        Клас: TaskRepository

        Методи:

        loadAll()
        saveAll()
        add(task)
        update(task)
        remove(id)
        Клас: SettingsRepository
        load()
        save()
        Передача даних:
        UI → DTO → Repository
        Repository → JSON file
13. Dirty state
        Коли true:
        змінено title/description/status
        змінено settings
        Коли false:
        після save
        після load
        Підтвердження потрібно при:
        закритті вікна
        переході без збереження
        скасуванні редагування
14. Результат здачі

        Документ містить:

        сутності Task і Settings
        поля та типи
        модель стану
        JSON структура
        валідація
        repository архітектура
        dirty state