import os
import sys
import subprocess

script_path = os.path.abspath(sys.argv[0])
personal_dict = '.aspell.en.pws'

def spellcheck(file_path):
    """
    Checks a file for spelling errors using aspell within the containerized context.

    Args:
        file_path (str): Path to the file to check.

    Prints:
        Each misspelled word along with its line number and line.
    Returns:
        int: Count of misspelled words in the file.
    """
    temp_file = '/tmp/input.txt'
    personal_dict = '.aspell.en.pws'
    trimmed_file_path = file_path.replace("/mnt/", "")
    print(f"Spell checking file: {trimmed_file_path}")

    misspelled_count = 0

    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            for line_number, line in enumerate(file, start=1):
                with open(temp_file, 'w', encoding='utf-8') as temp:
                    temp.write(line)

                # Run aspell on the line
                try:
                    command = [
                        "aspell", "list",
                        "--encoding=utf-8",
                        "--mode=markdown",
                        f"--home-dir=/mnt",
                        f"--personal={personal_dict}"
                    ]

                    with open(temp_file, 'r', encoding='utf-8') as temp_input:
                        result = subprocess.run(command, stdin=temp_input, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

                    if result.returncode != 0:
                        raise RuntimeError(f"Error running aspell: {result.stderr}")

                    misspelled_words = result.stdout.strip().split('\n')

                    for word in misspelled_words:
                        if word:
                            misspelled_count += 1
                            print(f"{trimmed_file_path}:{line_number}:misspelled word: {word}")
                            print(f"    line: {line.strip()}")

                except Exception as e:
                    print(f"Error processing line {line_number} in {file_path}: {e}")

    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
    except Exception as e:
        print(f"Error: {str(e)}")

    return misspelled_count

def main():
    directory_path = "/mnt/technical_reference_manual"
    total_misspelled = 0

    for root, dirs, files in os.walk(directory_path):
        for file in files:
            if file.endswith('.md'):
                file_path = os.path.join(root, file)
                total_misspelled += spellcheck(file_path)

    print(f"Total misspelled words: {total_misspelled}")
    if total_misspelled > 0:
        print(f"  Correct the spelling errors or add an excption to the aspell dictionary: {personal_dict} and try again.")
        print(f"  Use `make spellcheck` to do an interactive spell checking session with the tool aspell to correct the errors or add exceptions to the dictionary.")
        sys.exit(1)

if __name__ == "__main__":
    main()

