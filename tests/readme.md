pytest unit test

pip3 install ./req.txt
pytest --vv 
pytest --vv --cov src/

root
|---src
	|--- a.py
	|--- b.py
	|...
|---tests
	|--- test_a.py
	|--- test_b.py
	|--- __init__.py

[in root directory]
1. pytest [test.py]  - run the only specified test file
2. pytest - run the all test files in the [tests] folder.
