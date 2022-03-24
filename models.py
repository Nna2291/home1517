import os

from flask_bcrypt import generate_password_hash
from flask_login import UserMixin
from peewee import *

from dotenv import load_dotenv
load_dotenv()

DATABASE = MySQLDatabase(host=os.getenv('HOST'),
                         port=3306,
                         user=os.getenv('ROOT'),
                         password=os.getenv('PASSWORD'),
                         database=os.getenv('DB'))


class User(UserMixin, Model):
    login = CharField(unique=True)
    password = CharField(max_length=100)

    class Meta:
        database = DATABASE
        order_by = ('-joined_at',)

    @classmethod
    def create_user(cls, login, password):
        try:
            with DATABASE.transaction():
                cls.create(
                    login=login,
                    password=generate_password_hash(password)
                )
        except IntegrityError:
            raise ValueError('Пользователь уже существует')


def initialize():
    DATABASE.connect()
    DATABASE.create_tables([User], safe=True)
    DATABASE.close()
