# Automatically generates a case similar to that of balls_in_box.xml but with a much larger number of balls.
import math


class Ball:
    def __init__(self, x, y, r):
        self.x = x
        self.y = y
        self.r = r

    def to_xml(self):
        return f'  <rigid_body x="{self.x} {self.y}" theta="1.570796326794897" v="0.0 0.0" omega="0" rho="8.0" r="{self.r}" geo_idx="0"/>'


plane_size = 10
num_balls = 10000

grid_size = math.ceil(num_balls ** 0.5)
ball_sep = plane_size / grid_size * 2 * math.sqrt(2)
ball_size = 0.45 * ball_sep
balls = []
s2 = math.sqrt(2)
for i in range(grid_size):
    for j in range(grid_size):
        if len(balls) < num_balls:
            balls.append(
                Ball(-plane_size * 2 + ball_sep / 2 * s2 + (i + j) * ball_sep * s2 / 2, (i - j) * ball_sep * s2 / 2,
                     ball_size))
# balls = [Ball(-60,0,0.5)]

balls_xml = '\n'.join([ball.to_xml() for ball in balls])

planes_xml = \
    f"""
  <static_plane x="-{plane_size} -{plane_size}" n="0.70710678118654752440 0.70710678118654752440"/>
  <static_plane x="{plane_size} -{plane_size}" n="-0.70710678118654752440 0.70710678118654752440"/>
  <static_plane x="{plane_size} {plane_size}" n="-0.70710678118654752440 -0.70710678118654752440"/>
  <static_plane x="-{plane_size} {plane_size}" n="0.70710678118654752440 -0.70710678118654752440"/>
"""

result = \
    f"""
<rigidbody2d_scene>

  <camera center="0 0" scale="8.3225" fps="50" render_at_fps="0" locked="0"/>

  <integrator type="symplectic_euler" dt="0.01"/>

  <impact_operator type="gr" CoR="1.0" v_tol="1.0e-9" cache_impulses="0">
    <solver name="policy_iteration" max_iters="100" tol="1.0e-12"/>
  </impact_operator>

  <near_earth_gravity f="0.0 -10.0"/>

{planes_xml}

  <geometry type="circle" r="{ball_size}"/>

{balls_xml}

</rigidbody2d_scene>
"""

with open('balls_in_box_big.xml', 'w') as f:
    f.write(result)
