# sk engine physics system
? How it work
    the sk's physic use swept aabb(Axis align bound box) to solve collision

    each box have a velocity that will be use to calculate the collision
    if 2 box is currently colliding(overlaping) then the collision will not be solved 
    if 2 box may collide next frame then the collision will be solved

    SDL calculate time by ticks: 1000 ticks = 1 second
    the physic system will update every 25 ticks(40 time per sec) and this may change
    