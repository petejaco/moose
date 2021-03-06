# checking rotation of points by 45 deg about z axis in a SolutionUserObject for a 2D situation
[Mesh]
  # this is chosen so when i rotate through 45deg i get a length of "1" along the x or y direction
  type = GeneratedMesh
  dim = 2
  xmin = -0.70710678
  xmax = 0.70710678
  nx = 3
  ymin = -0.70710678
  ymax = 0.70710678
  ny = 3
[]

[UserObjects]
  [./solution_uo]
    type = SolutionUserObject
    mesh = square_with_u_equals_x.e
    timestep = 1
    nodal_variables = u
    rotation0_vector = '0 0 1'
    rotation0_angle = 45
    transformation_order = rotation0
  [../]
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./u_init]
    type = FunctionIC
    variable = u
    function = solution_fcn
  [../]
[]

[Functions]
  [./solution_fcn]
    type = SolutionFunction
    from_variable = u
    solution = solution_uo
  [../]
[]

[Kernels]
  [./diff]
    type = TimeDerivative
    variable = u
  [../]
[]

[Executioner]
  type = Transient

  solve_type = 'NEWTON'

  l_max_its = 800
  nl_rel_tol = 1e-10
  num_steps = 1
  end_time = 1
  dt = 1
[]

[Outputs]
  file_base = solution_function_rot4
  exodus = true
  [./console]
    type = Console
    perf_log = true
  [../]
[]
