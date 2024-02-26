import re

phoneNoPattern = r'\s010-\d{4}-\d{4}\s'
creditCardPattern = r'\s\d{4}-\d{4}-\d{4}-\d{4}\s'

text = open('text2.txt', 'r', encoding='UTF-8').readlines()

phoneCount = 0
cardCount = 0
for line in text:
    phoneCount += len(re.findall(phoneNoPattern, line))
    cardCount += len(re.findall(creditCardPattern, line))

print(phoneCount)
print(cardCount)