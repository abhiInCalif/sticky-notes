__author__ = 'abkhanna'
import web
import store
import json
from ast import literal_eval


urls = (
    '/sticky/board', 'BoardDetail',
    '/sticky/api/board', 'ApiBoard',
    '/sticky/api/postit', 'ApiPostIt',
)


class ApiBoard:
    def GET(self):
        web.header('Content-type', 'application/json')
        request_input = web.input(ownerId='', boardId='')
        owner_id = request_input.ownerId
        board_id = request_input.boardId
        if owner_id == '' or board_id == '':
            return web.badrequest()
        board_data = store.Board.get(owner_id, board_id)
        postit_data = store.PostIt.fetch(board_id)
        return json.dumps({'board': board_data, 'postits': postit_data})

    def POST(self):
        web.header('Content-type', 'application/json')
        request_input = web.input(owners=[], time='')
        owners = request_input.owners
        time = request_input.time
        if owners == [] or time == '':
            return web.badrequest()
        data = store.Board.create(owners, time)
        return json.dumps({'board_id': data})

class ApiPostIt:
    def POST(self):
        web.header('Content-type', 'application/json')
        request_input = web.input(text='', position='(-1,-1)', color='', picture='', audio='', url='', media_type='', board_id='')
        text = request_input.text
        position = literal_eval(request_input.position)
        color = request_input.color
        picture = request_input.picture
        audio = request_input.audio
        url = request_input.url
        media_type = request_input.media_type
        board_id = request_input.board_id
        if text == '' or position == (-1,-1) or color == '' or picture == '' or audio == '' or url == '' \
                or media_type == '' or board_id == '':
            return web.badrequest()

        data = store.PostIt.put(text, position, color, picture, audio, url, media_type, board_id)
        return json.dumps({'postit_id': data})


class BoardDetail:
    def GET(self):
        # shows the board page
        render = web.template.render('templates')
        return render.board()

if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()
