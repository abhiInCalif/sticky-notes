./test | xargs -I{} curl --request POST --include --header 'Content-Type:application/json' --data {} --no-buffer http://localhost:8080/sticky/api/postit/space
