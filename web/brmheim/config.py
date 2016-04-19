import configparser

def get_db_config():
  with open('config.ini', 'r') as content_file:
    content = content_file.read()
  # Leaving as an inline example of a config file…
  #content = """
  #[mysqld]
  # host = 127.0.0.1
  # user = foo
  # password = foo
  # db = foo
  # port = 3306
  #"""
  config = configparser.ConfigParser(allow_no_value=True)
  config.read_string(content)
  return config
