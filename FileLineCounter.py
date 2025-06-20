import os

# Расширения файлов для подсчёта строк
VALID_EXTENSIONS = {
    '.py', '.lua', '.cs', '.hpp', '.cpp', '.c', '.h',
    '.html', '.css', '.scss', '.less', '.xml', '.json', '.yaml', '.yml',
    '.sh', '.ps1', '.bat', '.cmd', '.fs', '.vb', '.sql',
    '.gitignore', '.sln', '.user', '.csproj', '.vcproj'
}

# Игнорируемые директории
IGNORE_DIRS = {'bin', 'obj', 'Release', 'Debug', 'libs', '.idea'}

total_lines = 0

for root, dirs, files in os.walk('.'):
    # Фильтрация игнорируемых директорий
    dirs[:] = [d for d in dirs if d.lower() not in IGNORE_DIRS]
    
    for file in files:
        file_path = os.path.join(root, file)
        ext = os.path.splitext(file)[1].lower()
        
        # Проверка расширения и исключение .dll
        if ext in VALID_EXTENSIONS and ext != '.dll':
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    total_lines += sum(1 for _ in f)
                print(f'+ {file_path}')
            except Exception:
                print(f'! Ошибка чтения: {file_path}')

print(f'\nОбщее количество строк: {total_lines}')