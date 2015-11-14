__author__ = 'abkhanna'
import web

urls = (
    '/sticky/board', 'BoardDetail',
)

class BoardDetail:
    def GET(self):
        # shows the board page
        render = web.template.render('templates')
        return render.board()

if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()
