__author__ = 'abkhanna'
from pymongo import MongoClient
import uuid

def database():
    client = MongoClient()
    db = client.postitdb
    return db

class Board:
    @staticmethod
    def create(owners, auto_save_time):
        # owners is a list of owners
        # auto_save_time is the last time the front end requested a save
        board_id = uuid.uuid1()
        for owner in owners:
            doc = {
                '_id': '/' + str(owner) + '/' + str(board_id),
                'savetime': auto_save_time,
                'owner': owner,
            }
            database().Board.insert_one(doc)

        return str(board_id)

    @staticmethod
    def fetch(owner_id):
        return list(database().Board.find({'owner': str(owner_id)}))

    @staticmethod
    def get(owner_id, board_id):
        cursor_list = list(database().Board.find({'_id': '/' + str(owner_id) + '/' + str(board_id)}))
        return cursor_list[0] if len(cursor_list) > 0 else {}

class PostIt:
    @staticmethod
    def put(text, position, color, picture, audio, url, media_type, board_id):
        # given this input, we should be able to create a sticky
        # the position is a tuple (x,y)
        # media_type is what allows the frontend to realize what the apropriate form to
        # display is.
        post_it_id = uuid.uuid1()
        (xpos, ypos, zpos) = position
        doc = {
            '_id': '/' + str(board_id) + '/' + str(post_it_id),
            'board_id': str(board_id),
            'text': text,
            'x_pos': xpos,
            'y_pos': ypos,
            'z_pos': zpos,
            'color': color,
            'picture': picture,
            'audio': audio,
            'url': url,
            'media_type': media_type
        }
        database().PostIt.update({'_id': '/' + str(board_id) + '/' + str(post_it_id)}, doc, upsert=True)
        return str(post_it_id)

    @staticmethod
    def fetch(board_id):
        return list(database().PostIt.find({'board_id': str(board_id)}))

    @staticmethod
    def get(board_id, note_id):
        cursor_list = list(database().Board.find({'_id': '/' + str(board_id) + '/' + str(note_id)}))
        return cursor_list[0] if len(cursor_list) > 0 else {}
