# importing packagesimport sqlalchemy as saimport pandas as pdimport requestsfrom datetime import datetime, date, timedelta  user = "andrustn"pwd = "andrustn6011"host = '10.18.54.68'database = 'andrustn'cnx_string = 'mysql+pymysql://'+user + ':'+pwd + '@' + host + '/' + databasecnxn = sa.create_engine(cnx_string)before = (datetime.now() - timedelta(hours=10)).strftime('%Y-%m-%dT%H:%M:%S.%f') # 10 hours to account for time differencetoday = date.today()SoQL = "https://data.montgomerycountymd.gov/resource/xhwt-7h2h.json?$query=SELECT * WHERE arrest_date >= " + "'" + before + "'"results = requests.get(SoQL)dictr = results.json()results_df = pd.json_normalize(dictr)   results_df['insert_dt'] = todayresults_df# Push to mySQL databaseresults_df.to_sql(name = 'Arrests',con = cnxn, if_exists = 'append', index = False)#create table Arrests (last_name varchar(100), first_name varchar(100), age int, street varchar(100), city varchar(100), state varchar(2), arrest_date date, offense varchar(100), middle_name varchar(100), insert_dt varchar(100)   #);