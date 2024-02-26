import re

socialSecurityNoPattern = r'\s((98|99|01)((0[13578]|1[02])(0[1-9]|[12]\d|30|31)|(0[468]|11)(0[1-9]|[12]\d|30)|02(0[1-9]|1\d|2[0-8]))|00((0[13578]|1[02])(0[1-9]|[12]\d|30|31)|(0[468]|11)(0[1-9]|[12]\d|30)|02(0[1-9]|1\d|2[0-9])))-[1-4]\d{6}\s'
emailPattern = r'\s[a-zA-Z0-9]+@[a-zA-Z]+\.ac\.kr\s'

text = open('text2.txt', 'r', encoding='UTF-8').readlines()

ssnCount = 0
emailCount = 0
for line in text:
    ssnCount += len(re.findall(socialSecurityNoPattern, line))
    emailCount += len(re.findall(emailPattern, line))

print(ssnCount)
print(emailCount)