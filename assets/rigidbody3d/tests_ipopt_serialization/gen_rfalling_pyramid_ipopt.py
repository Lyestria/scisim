# Automatically generates a case similar to that of balls_in_box.xml but with a much larger number of balls.
import math
import os


class Ball:
    def __init__(self, x, y, z, v):
        self.x = x
        self.y = y
        self.z = z
        self.v = v

    def to_xml(self):
        return f'  <rigid_body_with_density x="{self.x} {self.y} {self.z}" R="0.0 0.0 0.0" v="{self.v[0]} {self.v[1]} {self.v[2]}" omega="0.0 0.0 0.0" rho="1.0" fixed="0" geo_idx="0"/>'


def create_xml(size):
    r = 0.5

    balls = []
    s2 = math.sqrt(2)
    for i in range(size):
        for j in range(i+1):
            for k in range(i+1):
                balls.append(Ball(s2*i*r, (j-i/2)*2*r, (k-i/2)*2*r, (0, 0, 0)))

    balls_xml = '\n'.join([ball.to_xml() for ball in balls])

    planes_xml = \
        f"""
<static_plane x="{-1.01*r} 0 0" n="1 0 0"/>

"""

    result = \
        f"""
        <rigidbody3d_scene>
  <camera_perspective theta="-1.5707" phi="1.5707" rho="7.12621" lookat="0 0 0" up="0 1 0" fps="50" render_at_fps="0" locked="0"/>
    <near_earth_gravity f="-5 0 0"/>
  <integrator type="split_ham" dt="0.01"/>
  
<impact_operator type="grr" CoR="1.0">
    <elastic_operator type="gr" CoR="1.0" v_tol="1.0e-6">
      <solver name="ipopt" linear_solvers="ma97 ma57 mumps ma27 ma86" tol="1.0e-12"/>
    </elastic_operator>
    <inelastic_operator type="lcp" CoR="0.0">
      <solver name="ipopt" linear_solvers="ma97 ma57 mumps ma27 ma86" tol="1.0e-12"/>
    </inelastic_operator>
  </impact_operator>

    
    {planes_xml}
    
      <geometry type="sphere" r="{r}"/>
    
    {balls_xml}
    
    </rigidbody3d_scene>
    """

    os.makedirs('rfalling_pyramid_ipopt', exist_ok=True)
    with open(os.path.join('rfalling_pyramid_ipopt', f'{size}.xml'), 'w') as f:
        f.write(result)

sizes = [2, 5, 10, 20, 30, 40, 50]
for size in sizes:
    create_xml(size)