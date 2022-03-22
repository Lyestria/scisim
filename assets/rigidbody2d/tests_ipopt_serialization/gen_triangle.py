# Automatically generates a case similar to that of balls_in_box.xml but with a much larger number of balls.
import math
import os

class Ball:
    def __init__(self, x, y, r, v=(0, 0)):
        self.x = x
        self.y = y
        self.r = r
        self.v = v

    def to_xml(self):
        return f'  <rigid_body x="{self.x} {self.y}" theta="1.570796326794897" v="{self.v[0]} {self.v[1]}" omega="0" rho="8.0" r="{self.r}" geo_idx="0"/>'


def create_xml(triangle_size):
    ball_size = 0.5

    balls = [Ball(-2 * ball_size, 0, ball_size, (5, 0))]
    s3 = math.sqrt(3)
    for i in range(triangle_size):
        for j in range(i+1):
            balls.append(Ball(ball_size * s3 * i, ball_size * (- i + 2 * j), ball_size))

    balls_xml = '\n'.join([ball.to_xml() for ball in balls])

    planes_xml = ''

    result = \
        f"""
    <rigidbody2d_scene>
    
      <camera center="0 0" scale="8.3225" fps="50" render_at_fps="0" locked="0"/>
    
      <integrator type="symplectic_euler" dt="0.01"/>
    
      <impact_operator type="gr" CoR="1.0" v_tol="1.0e-9" cache_impulses="0">
        <solver name="policy_iteration" max_iters="100" tol="1.0e-6"/>
      </impact_operator>
    
    
    {planes_xml}
    
      <geometry type="circle" r="{ball_size}"/>
    
    {balls_xml}
    
    </rigidbody2d_scene>
    """
    os.makedirs('triangle', exist_ok=True)
    with open(os.path.join('triangle', f'{triangle_size}.xml'), 'w') as f:
        f.write(result)


sizes = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 125, 150, 200, 500, 1000]
for size in sizes:
    create_xml(size)
