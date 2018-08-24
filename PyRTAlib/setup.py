from setuptools import setup

requires = [
    'redis==2.10.6',
    'mysql-connector-python==8.0.12',
    'astropy==3.0.4'
]
setup(
    install_requires = requires,
)
