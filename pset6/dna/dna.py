import csv
import sys
import re

if len(sys.argv) != 3:
    print("Usage: python dna.py <database csv> <person txt>")
    sys.exit(1)

csv_f = sys.argv[1]
txt_f = sys.argv[2]

c = 0
people = []
sequences = []

try:
    with open(csv_f) as csv_file:
        reader = csv.reader(csv_file, delimiter=',', quoting=csv.QUOTE_NONE)
        for row in reader:
            if c == 0:
                sequences = row
                c += 1
            else:
                d = 0
                person = {}
                for item in row:
                    if d == 0:
                        person['name'] = item
                        x = person['sequences'] = {}
                    else:
                        x[sequences[d]] = int(item)
                    d += 1
                people.append(person)
except Exception as e:
    print("Error: " + str(e))
    sys.exit(1)

# number of fields with different sequences in csv file
seq_no = len(sequences) - 1

# read the entire text file into memory
try:
    with open(txt_f, 'r') as txt_file:
        txt = txt_file.read()
except Exception as e:
    print("Error: " + str(e))
    sys.exit(1)


# this function is finding subsequent occurences of string in string
# it is very fast because it's using re C library
def strrep(txt, seq):
    ret = max(re.findall('((?:' + re.escape(seq) + ')*)', txt), key=len)
    return int(len(ret) / len(seq))


# simple human fiendly loop to search for matches
# as per specification, if person is found, it exists
for person in people:
    sequences = person['sequences']
    name = person['name']
    match_count = 0
    for seq, occ in sequences.items():
        no = strrep(txt, seq)
        if no == occ:
            match_count += 1
    if match_count == seq_no:
        print(name)
        sys.exit(0)

print("No match")
