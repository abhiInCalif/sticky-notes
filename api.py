__author__ = 'abkhanna'
import web
import store
import json
from ast import literal_eval


urls = (
    '/sticky/board', 'BoardDetail',
    '/sticky/api/board', 'ApiBoard',
    '/sticky/api/postit', 'ApiPostIt',
    '/sticky/api/postit/space', 'ApiPostItFromSpaceTouch',
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

class ApiPostItFromSpaceTouch:
    def POST(self):
        web.header('Content-type', 'application/json')
        data = web.data()
        if "Tap" not in data:
            print "ignored"
        else:
            # we have a tap so lets do something with it.
            print "Received tap: {0}".format(data)
            # Tap began, x:0.728088; y:0.500000; z:246.762131
            whitespace_split = data.split(" ")
            xs = whitespace_split[2].split(":")[1][:-1]
            ys = whitespace_split[3].split(":")[1][:-1]
            zs = whitespace_split[4].split(":")[1][:-1]

            text = "David's Friend is Awesome!"
            position = literal_eval("(" + xs + "," + ys + "," + zs + ")")
            color = "red"
            picture = "default"
            audio = "default"
            url = "default"
            media_type = "default"
            board_id = "abhi1994/7d9d2cb8-8aaa-11e5-a3b2-a0999b160459"
            data = store.PostIt.put(text, position, color, picture, audio, url, media_type, board_id)


class BoardDetail:
    def GET(self):
        # shows the board page
        render = web.template.render('templates')
        return render.stickynotes()

if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()
