$ = jQuery

init = ->
    $.get "is_ready", (data) ->
        aud = $("#test_player")[0]
        $("#test_source").attr "src", data
        aud.load()
        aud.play()
