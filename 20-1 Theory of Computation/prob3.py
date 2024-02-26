import re

socialSecurityNoPattern = r'\s((98|99|01)((0[13578]|1[02])(0[1-9]|[12]\d|30|31)|(0[468]|11)(0[1-9]|[12]\d|30)|02(0[1-9]|1\d|2[0-8]))|00((0[13578]|1[02])(0[1-9]|[12]\d|30|31)|(0[468]|11)(0[1-9]|[12]\d|30)|02(0[1-9]|1\d|2[0-9])))-[1-4](\d{6})\s'
emailPattern = r'\s[a-zA-Z0-9]+@[a-zA-Z]+\.ac\.kr\s'

text = open('text2.txt', 'r', encoding='UTF-8').readlines()

ssnList = []
emailList = []

for line in text:
    for m in re.finditer(socialSecurityNoPattern, line):
        ssnList.append(m.group().strip())
    for m in re.finditer(emailPattern, line):
        emailList.append(m.group().strip())

for ssn in ssnList:
    modifiedssn = re.sub(r'(\d)(\d{6})', r'\1XXXXXX', ssn)
    print(modifiedssn)

for email in emailList:
    modifiedemail = re.sub(r'(\w+@)([a-zA-Z]+)(\.ac\.kr)', r'\1XXXX\3', email)
    print(modifiedemail)