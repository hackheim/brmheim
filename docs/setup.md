# Python3.5

Install python 3.5  .. put instructions

wget https://www.python.org/ftp/python/3.5.1/Python-3.5.1.tar.xz
tar -Jxvf Python-3.5.1.tar.xz
./configure --prefix /opt/py3
make
make test  # Probably will have some tests failing, but ensure it looks mostly ok :')
make install # Ensure you have write access to /opt/py3

# Setup Virtualenv

virtualenv --no-site-packages --python python3.5 ~/.virtualenvs/brmheim
source ~/.virtualenvs/brmheim/bin/activate
# If you used local python prefix install, you need to upgrade setuptools after entering it's virtualenv!
pip install -U setuptools

pip install -r requirements.txt

