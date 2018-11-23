from setuptools import setup

requires = [
    'redis==2.10.6',
    'mysql-connector-python==8.0.12',
    'astropy==3.0.4',
    'coverage==5.0a3',
    'matplotlib==3.0.2'
]
setup(
    install_requires = requires,
)
