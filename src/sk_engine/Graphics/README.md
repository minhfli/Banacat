sk_engine 
sk_graphic

this folder contain some opengl classes and a 2d sprite-batch and line renderer

2d sprite batch renderer implementation:

    only 1 active texture
    when a quad with new texture is added, flush all quad with old texture
    
    you will want to minimize draw calls
    -> try to draw quads with same texture continuously
    
    even if you draw 1 one texute at a time, it still pretty fast
    testing result: (only draw, no game logic)

        -draw 1 texture at a time (bind texture and draw and again)
            300fps with 500 quads
            120fps      1600 quads
            55fps       4225 quads

    the fragment shader use telxelFetch() function, not texture()
        pros:
            completely advoid texture bleeding when using sprite-sheet(texture atlas)
            faster
        cons:
            no filtering 
            no mipmaping
            >flickering when drawing large texture in small size
            >no antialiasing (but this is a 2d pixel engine anyway)

    gl depth test is enabled
    only use texture with pixel alpha alway equal 0 or 1,



