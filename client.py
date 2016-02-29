#Sagar Saija

from __future__ import print_function
import csv
import sys
import re
import mysql.connector
import subprocess

config = {
  'user': 'root',
  'password': 'cs174$',
  'host': '54.153.106.24',
  'port': '3306',
  'database': 'project',
  'buffered': True,
}


try:
  cnx = mysql.connector.connect(**config)
  cursor = cnx.cursor()
except mysql.connector.Error as err:
  print("Connection Error: {}".format(err))
  sys.exit(1)

query = open("query.txt", "r")
call = query.readline()
call = call.upper()
sql_call = call.split()
numerator = []
denominator = 0

def execute(query, values):
  print("Executing: {} ... ".format(query % values), end=" ")
  try:
    cursor.execute(query, values)
  except mysql.connector.Error as err:
    print("ERROR\nMySQL Error: {}\n".format(err))
    sys.exit(1)
  else:
    print("Success")

def execute_insert(call):
  print("p1")
  encrypt = "./encOutput " + call[3]
  print(encrypt)
  subprocess.check_call(encrypt, shell = True)
  print("p2")
  encryptedData = open("encrypted.txt", "r")
  encSalary = encryptedData.readline()
  print("p2")
  call[3] = encSalary
  insert = ("INSERT INTO Employees VALUES""(%(id)s, %(age)s, %(salary)s)")
  keys = ['id', 'age', 'salary']
  values = dict(zip(keys, call[1:4]))
  execute(insert, values)

def execute_select(call):
  if(call[1] == "*"):
    select = ("SELECT * FROM Employees")
    cursor.execute(select)
    printFetch()
    #do something
  elif(re.match("[0-9*]", call[1])):
    select = ("SELECT * FROM Employees WHERE id=" + call[1])
    cursor.execute(select)
    printFetch()
  elif(call[1] == "SUM"):
    if any("GROUP" in s for s in call):
      grp = call.index("BY") + 1
      select = ("SELECT " + call[grp] + ", SUM(salary) FROM Employees " + " ".join(call[2:len(call)]))
      print(select)
      cursor.execute(select)
      printFetch()
    else:
      select = ("SELECT SUM(salary) FROM Employees " + " ".join(call[2:len(call)]))
      print(select)
      cursor.execute(select)
      printFetch()
  elif(call[1] == "AVG"):
    if any("GROUP" in s for s in call):
      grp = call.index("BY") + 1
      select = ("SELECT " + call[grp] + ", SUM(salary) FROM Employees " + " ".join(call[2:len(call)]))
      print(select)
      cursor.execute(select)
      getNumerator(1)
      results = cursor.fetchall()
      count = ("SELECT COUNT(*) FROM Employees " + " ".join(call[2:len(call)]))
      cursor.execute(count)
      getDenominator()
    else:
      select = ("SELECT SUM(salary) FROM Employees " + " ".join(call[2:len(call)]))
      print(select)
      cursor.execute(select)
      getNumerator(0)
      count = ("SELECT COUNT(*) FROM Employees " + " ".join(call[2:len(call)]))
      cursor.execute(count)
      getDenominator()


def printFetch():
  results = cursor.fetchall()
  for row in results:
    print(" ".join(str(v) for v in row))

def getNumerator(grp):
  results = cursor.fetchall()
  for row in results:
    print(" ".join(str(v) for v in row))
    global numerator
    numerator.append(int(row[grp]))
    print("n: " + str(row[grp]))

def getDenominator():
  i = 0;
  results = cursor.fetchall()
  for row in results:
    global denominator
    denominator = int(row[0])
    print(str(numerator[i]/denominator))
    i += 1

if(sql_call[0] == "INSERT"):
  execute_insert(sql_call)
#
#  # Commit data
if(sql_call[0] == "SELECT"):
  execute_select(sql_call)


cnx.commit()
query.close()
cursor.close()
cnx.close()
