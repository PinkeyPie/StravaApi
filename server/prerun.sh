p=0
command -v python3 >/dev/null 2>&1 && p=1
if [ $p -eq 1 ]
then
  python3 -m venv venv
  source venv/bin/activate
  pip install -r requirements.txt
else
  echo "Python is not installed"
fi