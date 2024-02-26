import psycopg2

connect = psycopg2.connect("dbname=tutorial user=postgres password=0000")
cur = connect.cursor()  # create cursor

cur.execute("SELECT * FROM users;")
result = cur.fetchall()  # you must use cur.fetchall() when read data from PostgreSQL
print(result)
print("=====")
print(result[0])
print("=====")
print(result[0][0])
