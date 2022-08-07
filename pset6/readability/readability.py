
import string

sentence = input("Text: ")

letters = 0
words = 1
sentences = 0

for letter in sentence:
    if letter == "!" or letter == "?" or letter == ".":
        sentences += 1
    elif letter in string.punctuation:
        continue
    elif letter in string.whitespace:
        words += 1
    else:
        letters += 1

#  liau index
wordFactor = words / 100
lettersPer100 = letters / wordFactor
sentencesPer100 = sentences / wordFactor
liauIndex = round((0.0588 * lettersPer100) - (0.296 * sentencesPer100) - 15.8)

# Print grade

if liauIndex > 16:
    print("Grade 16+")
elif liauIndex < 1:
    print("Before Grade 1")
else:
    print(f"Grade {liauIndex}")
