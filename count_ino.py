from pathlib import Path

count = len(list(Path(".").rglob("*.ino")))
print(f"Number of .ino files: {count}")
