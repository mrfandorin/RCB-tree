@shocktube = true
@shearcavity = false
@dim = 1
@summation_density = false
@virtual_part = true
@nnps = 1
@skf = 1
@nor_density = true
@print_step = 5

@visc = true
@visc_artificial = true
@pa_sph= 2

def time_print
  puts Time.now
end

def shock_tube(x, vx, mass, rho, p, u, itype, hsml, ntotal)
  ntotal = 400
  space_x = 0.6/80
  mass = []
  hsml = []
  itype = []
  x = []
  u = []
  rho = []
  p = []
  for i in 0 ... ntotal
    mass[i] = 0.75/400
    hsml[i] = 0.015
    itype[i] = 1
    x[i] = []
    vx[i] = []
    for j in 0 ... @dim
      x[i][j] = 0
      vx[i][j] = 0
    end
  end

  for i in 0 ... 320
    x[i][0] = -0.6 + space_x/4 * (i - 1)
  end

  for i in 320 + 1 ... ntotal
    x[i][0] = space_x * (i - 320)
  end

  for i in 0 ... ntotal
    if x[i][0] < 1.0e-8
      u[i] = 2.5
      rho[i] = 1
      p[i] = 1
    end
    if x[i][0] > 1.0e-8
      u[i] = 1.795
      rho[i] = 0.25
      p[i] = 0.1795
    end
  end

  return [x, vx, mass, rho, p, u, itype, hsml, ntotal]
end

def input()

  ntotal = 5
  mass = [1, 2, 1, 3, 5]
  itype = [1, 1, 1, 1, 1]
  x = [[1, 2, 1], [1, 2, 2], [1, 2, 3], [1, 2, 4], [1, 2, 5]]
  vx = [[1, 2, 1], [1, 2, 2], [1, 2, 3], [1, 2, 4], [1, 2, 5]]
  rho = [0.3, 0.3, 0.3, 0.3, 0.4]
  p = [0.3, 0.3, 0.3, 0.3, 0.4]
  u = [0.3, 0.3, 0.3, 0.3, 0.4]
  hsml = [0.3, 0.3, 0.3, 0.3, 0.4]

  x, vx, mass, rho, p, u, itype, hsml, ntotal = shock_tube(x, vx, mass, rho, p, u, itype, hsml, ntotal) if @shocktube
  
  return [x, vx, mass, rho, p, u, itype, hsml, ntotal]
end

def virt_part(itimestep, ntotal, nvirt, hsml, mass, x, vx, rho, u, p, itype)
  
  nvirt = 0
  mp = 40
  xl = 1.0e-3
  dx = xl / mp
  v_inf = 1.0e-3 
  

  #Upper side
  for i in 0 ... 2 * mp + 1
    nvirt += 1
    x[ntotal + nvirt][0] = i * dx / 2
    x[ntotal + nvirt][1] = xl
    vx[ntotal + nvirt][0] = v_inf
    vx[ntotal + nvirt][1] = 0
  end

  # Lower side
  for i in 0 ... 2 * mp + 1
    nvirt += 1
    x[ntotal + nvirt][0] = i * dx / 2
    x[ntotal + nvirt][1] = 0
    vx[ntotal + nvirt][0] = 0
    vx[ntotal + nvirt][1] = 0
  end

  #Left side
  for i in 0 ... 2 * mp - 1
    nvirt += 1
    x[ntotal + nvirt][0] = 0
    x[ntotal + nvirt][1] = (i + 1) * dx / 2
    vx[ntotal + nvirt][0] = 0
    vx[ntotal + nvirt][1] = 0
  end

  #Right side
  for i in 0 ... 2 * mp - 1
    nvirt += 1
    x[ntotal + nvirt][0] = xl
    x[ntotal + nvirt][1] = (i + 1) * dx / 2
    vx[ntotal + nvirt][0] = 0
    vx[ntotal + nvirt][1] = 0
  end

  for i in 0 ... nvirt
    rho[ntotal + i] = 1000
    mass[ntotal + i] = rho[ntotal + i] * dx * dx
    p[ntotal + i] = 0
    u[ntotal + i] = 357.0
    itype[ntotal + i] = -2
    hsml[ntotal + i] = dx
  end
  
  return [nvirt, hsml, mass, x, vx, rho, u, itype]
end

def direct_find(itimestep, ntotal + nvirt, hsml, x)
  pair_i = []
  pair_j = []
  countiac = []
  w = []
  dwdx = []
  dxiac = []  
  max_interaction = 5

  if @skf == 1
    scale_k = 2
  elsif @skf == 2
    scale_k = 3
  elsif @skf == 3
    scale_k = 3
  end

  for i in 0 ... ntotal
    countiac = 0
  end
  
  niac = 0

  for i in 0 ... ntotal - 1
    for j in i + 1 ... ntotal
      dxiac[0] = x[i][0] - x[j][0]
      driac = dxiac[0] * dxiac[0]
      for d in 1 ... @dim
        dxiac[d] = x[i][d] - x[j][d]
        driac = driac + dxiac[d] * dxiac[d]
      end
      mhsml = (hsml[i] + hsml[j]) / 2.0
      if Math.sqrt(driac) < scale_k * mhsml
        if niac < max_interaction
          niac += 1
          pair_i[niac] = i
          pair_j[niac] = j
          r = Math.sqrt(driac)
          countiac[i] += 1
          countiac[j] += 1
        else
          puts "Error: direct_find, too many interactions"
        end
      end
    end
  end
 
  # Statistics for the interactions
  sumiac = 0
  maxiac = 0
  miniac = 1000
  noiac = 0
  
  for i in 0 ... ntotal
    sumiac += countiac[i]
    if countiac[i] > maxiac
      maxiac = countiac[i]
      maxp = i 
    end
    if countiac[i] < miniac
      miniac = countiac[i]
      minp = i
    end  
    noiac += 1 if countiac[i] == 0
  end
  
  if itimestep % @print_step == 0
    puts "Statistics: interactions per particle"
    puts "Particle: #{maxp}, maximal interactions: #{maxiac}"
    puts "Particle: #{minp}, minimal interactions: #{miniac}"
    puts "Total pairs: #{niac}"
    puts "Particle with no interactions: #{noiac}"
  end
    
  return [niac, pair_i, pair_j, w, dwdx, countiac]
end

def kernel(r, dx, hsml)
  dwdx = []
  
  q = r / hsml
  w = 0.0
  
  for d in 0 ... @dim
    dwdx[d] = 0.0
  end
  
  skf = 1
  if skf == 1
    if @dim == 1
      factor = 1.0 / hsml
    elsif @dim == 2
      factor = 15.0 / (7 * Math::PI * hsml * hsml)
    elsif @dim == 3
      factor = 3.0 / (2 * Math::PI * hsml * hsml * hsml)
    end  

    if q > 0.0 && q < 1.0    
      w = factor * (2.0 / 3.0 - q * q + q**3 / 2.0)
      for d in 0 ... @dim
        dwdx[d] = factor * (-2.0 + 3.0 / 2.0 * q) / hsml**2 * dx[d]
      end
    elsif q > 1.0 && q < 2
      w = factor * 1.0 / 6.0 * (2.0 * -q) ** 3
      for d in 0 ... @dim
        dwdx[d] = -factor * 1.0 / 6.0 * 3.0 * (2.0 * -q)**2 / hsml * (dx[d] / r)
      end
    else
      w = 0
      for d in 0 ... @dim
        dwdx[d] = 0
      end
    end
  elsif skf == 2
    factor = 1.0 / (hsml**@dim * Math.PI**(@dim/2.0))
    if q > 0 && q < 3
      w = factor * Math::E**(-q * q)
      for d in 0 ... @dim
        dwdx[d] = w * (-2.0 * dx[d] / hsml / hsml)
      end
    else
      w = 0
      for d in 0 ... @dim
        dwdx[d] = 0
      end
    end
  elsif skf == 3
    if @dim == 1
      factor = 1.0 / (120.0 * hsml)
    elsif @dim == 2
      factor = 7.0 / (478.0 * Math::PI * hsml**2)
    elsif @dim == 3
      factor = 1.0 / (120.0 * Math::PI * hsml**3)
    end

    if q > 0 && q < 1
      w = factor * ((3 - q)**5 - 6 * (2 - q)**5 + 15 * (1 - q)**5)
      for d in 0 ... @dim
        dwdx[d] = factor * ((-120 + 120 * q - 50 * q**2) / hsml**2 * dx[d])
      end
    elsif q > 1 && q < 2
      w = factor * ((3 - q)**5 - 6 * (2 - q)**5)
      for d in 0 ... @dim
        dwdx[d] = factor * (-5 * (3 - q)**4 + 30 * (2 - q)**4) / hsml * (dx[d] / r)
      end
    elsif q > 2 && q < 3
      w = factor * (3 - q)**5
      for d in 0 ... @dim
        dwdx[d] = factor * (-5 * (3 - q)**4) / hsml * (dx[d] / r)
      end
    else
      w = 0
      for d in 0 ... @dim
        dwdx[d] = 0
      end
    end
  end  
    
  return [w, dwdx]
end  

def sum_density(ntotal, hsml, mass, niac, pair_i, pair_j, w, itype, rho)
  hv = []
  wi = []
  
  for d 0 ... @dim
    hv[d] = 0.0
  end  
  
  r = 0.0
  
  for i in 0 ... ntotal
    selfdens, hv = kernel(r, hv, hsml[i])
    wi[i] = selfdens * mass[i] / rho[i]
  end  
  
  for k in 0 ... niac
    i = pair_i[k]
    j = pair_j[k]
    wi[i] += mass[j] / rho[j] * w[k]
    wi[j] += mass[i] / rho[i] * w[k]
  end  
  
  for i in 0 ... ntotal
    selfdens, hv = kernel(r, hv, hsml[i])
    rho[i] = selfdens * mass[i]
  end
  
  for k in 0 ... niac
    i = pair_i[k]
    j = pair_j[k]
    rho[i] += mass[j] * w[k]
    rho[j] += mass[i] * w[k]
  end

  if @nor_density
    for i in 1 ... ntotal
      rho[i] = rho[i] / wi[i]
    end
  end
      
  return rho
end

def com_density(ntotal, mass, niac, pair_i, pair_j, dwdx, vx)
  drhodt = []
  dvx = []
  
  for i in 0 ... ntotal
    drhodt[i] = 0
  end

  for k in 0 ... niac
    i = pair_i[k]
    j = pair_j[k]
    for d in 0 ... @dim
      dvx[d] = vx[i][d] + vx[j][d]
    end
    vcc = dvx[0] * dwdx[k][0]
    for d in 1 ... @dim
      vcc += dvx[d] * dwdx[k][d]
    end
    drhodt[i] = drhodt[i] + mass[j] * vcc
    drhodt[j] = drhodt[j] + mass[i] * vcc
  end

  return drhodt
end

def viscosity(ntotal, itype, x, rho)
  eta = []

  for i in 0 ... ntotal
    if itype[i].abs == 1
      eta[i] = 0
    elsif itype[i].abs == 2
      eta[i] = 1.0e-3
    end
  end

  return eta
end

def p_gas(rho, u)
  gamma = 1.4
  p = (gamma - 1) * rho * u
  c = Math.sqrt((gamma - 1) * u)

  return [p, c]
end

def p_art_water(rho, u)
  gamma = 7.0
  rho0 = 1000.0
  b = 1.013e5
  p = b * ((rho / rho0)**gamma - 1)
  c = 1480

  #c = 0.01
  #p = c**2 * rho
  return [p, c]
end

def int_force(itimestep, dt, ntotal, hsml, mass, vx, niac, rho, eta, pair_i, pair_j, dwdx, u, itype, x, t)
  c = []
  p = []
  dvxdt = []
  tdsdt = []
  dedt = []

  dvx = []
  txx = []
  tyy = []
  tzz = []
  txy = []
  txz = []
  tyz = []
  vcc = []
  
  for i in 0 ... ntotal
    txx[i] = 0.0
    tyy[i] = 0.0
    tzz[i] = 0.0
    txy[i] = 0.0
    txz[i] = 0.0
    tyz[i] = 0.0 
    vcc[i] = 0.0
    tdsdt[i] = 0.0
    dedt[i] = 0.0
    for d in 0 ... @dim
      dvxdt[i][d] = 0.0
    end
  end

  if @visc
    for k in 0 ... niac
      i = pair_i[k]
      k = pair_j[k]
      for d in 0 ... @dim
        dvx[d] = vx[j][d] - vx[i][d]
      end
      if @dim == 1
        hxx = 2.0 * dvx[0] * dwdx[k][0]
      elsif @dim == 2
        hxx = 2.0 * dvx[0] * dwdx[k][0] - dvx[1] * dwdx[k][1]
        hxy = dvx[0] * dwdx[k][1] + dvx[1] * dwdx[k][0]
        hyy = 2.0 * dvx[1] * dwdx[k][1] - dvx[0] * dwdx[k][0]
      elsif @dim == 3
        hxx = 2.0 * dvx[0] * dwdx[k][0] - dvx[1] * dwdx[k][1] - dvx[2] * dwdx[k][2]
        hxy = dvx[0] * dwdx[k][1] + dvx[1] * dwdx[k][0]
        hxz = dvx[0] * dwdx[k][2] + dvx[2] * dwdx[k][0]        
        hyy = 2.0 * dvx[1] * dwdx[k][1] - dvx[0] * dwdx[k][0]
        hyz = dvx[1] * dwdx[k][2] + dvx[2] * dwdx[k][1]
        hzz = 2.0 * dvx[2] * dwdx[k][2] - dvx[0] * dwdx[k][0] - dvx[1] * dwdx[k][1]
      end
      hxx = 2.0 / 3.0 * hxx
      hyy = 2.0 / 3.0 * hyy
      hzz = 2.0 / 3.0 * hzz

      if @dim == 1
        txx[i] = txx[i] + mass[j] * hxx / rho[j]
        txx[j] = txx[j] + mass[i] * hxx / rho[i]
      elsif @dim == 2
        txx[i] = txx[i] + mass[j] * hxx / rho[j]
        txx[j] = txx[j] + mass[i] * hxx / rho[i]
        txy[i] = txy[i] + mass[j] * hxy / rho[j]
        txy[j] = txy[j] + mass[i] * hxy / rho[i]
        tyy[i] = tyy[i] + mass[j] * hyy / rho[j]
        tyy[j] = tyy[j] + mass[i] * hyy / rho[i]
      elsif @dim == 3
        txx[i] = txx[i] + mass[j] * hxx / rho[j]
        txx[j] = txx[j] + mass[i] * hxx / rho[i]
        txy[i] = txy[i] + mass[j] * hxy / rho[j]
        txy[j] = txy[j] + mass[i] * hxy / rho[i]
        txz[i] = txz[i] + mass[j] * hxz / rho[j]
        txz[j] = txz[j] + mass[i] * hxz / rho[i]
        tyy[i] = tyy[i] + mass[j] * hyy / rho[j]
        tyy[j] = tyy[j] + mass[i] * hyy / rho[i]
        tyz[i] = tyz[i] + mass[j] * hyz / rho[j]
        tyz[j] = tyz[j] + mass[i] * hyz / rho[i]
        tzz[i] = tzz[i] + mass[j] * hzz / rho[j]
        tzz[j] = tzz[j] + mass[i] * hzz / rho[i]
      end

      hvcc = 0
      for d in 0 ... @dim
        hvcc = hvcc + dvx[d] * dwdx[k][d]
      end 
      vcc[i] = vcc[i] + mass[j] * hvcc / rho[j]
      vcc[j] = vcc[j] + mass[i] * hvcc / rho[i]
    end

  end


  for i in 0 ... ntotal
    if @visc
      if @dim == 1
        tdsdt[i] = txx[i] * txx[i]
      elsif @dim == 2
        tdsdt[i] = txx[i] * txx[i] + 2.0 * txy[i] * txy[i] + tyy[i] * tyy[i]
      elsif @dim == 3
        tdsdt[i] = txx[i] * txx[i] + 2.0 * txy[i] * txy[i] + 2.0 * txz[i] * txz[i] + tyy[i] * tyy[i] + 2.0 * tyz[i] * tyz[i] + tzz[i] * tzz[i]
      end
      tdsdt[i] = 0.5 * eta[i] / rho[i] * tdsdt[i]
    end

    if itype[i].abs == 1
      p[i], c[i] = p_gas(rho[i], u[i])
    elsif itype[i].abs == 2
      p[i], c[i] = p_art_water(rho[i], u[i])
    end  
  end 

  for k in 0 ... niac
    i = pair_i[k]
    j = pair_j[k]
    he = 0.0

    rhoij = 1.0 / (rho[i] * rho[j])
    if @pa_sph == 1
      for d in 0 ... @dim
        h = - (p[i] + p[j]) * dwdx[k][d]
        he = he + (vx[j][d] - vx[i][d]) * h
        if @visc
          if d == 0
            h = h + (eta[i] * txx[i] + eta[j] * txx[j]) * dwdx[k][0]
            if @dim > 2
              h = h + (eta[i] * txy[i] + eta[j] * txy[j]) * dwdx[k][1]
              if @dim == 3
                h = h + (eta[i] * txz[i] + eta[j] * txz[j]) * dwdx[k][2]
              end
            end 
          elsif d == 1
            h = h + (eta[i] * txy[i] + eta[j] * txy[j]) * dwdx[k][0] + (eta[i] * tyy[i] + eta[j] * tyy[j]) * dwdx[k][1]
            if @dim == 3
              h = h + (eta[i] * tyz[i] + eta[j] * tyz[j]) * dwdx[k][2]
            end
          elsif d == 2
             h = h + (eta[i] * txz[i] + eta[j] * txz[j]) * dwdx[k][0] + (eta[i] * tyz[i] + eta[j] * tyz[j]) * dwdx[k][1] + (eta[i] * tzz[i] + eta[j] * tzz[j]) * dwdx[k][2]
          end
        end
        h = h * rhoij
        dvxdt[i][d] = dvxdt[i][d] + mass[j] * h
        dvxdt[j][d] = dvxdt[j][d] - mass[i] * h
      end
      he = he * rhoij
      dedt[i] = dedt[i] + mass[j] * he
      dedt[j] = dedt[j] + mass[i] * he
    elsif @pa_sph == 2
      for d in 0 ... @dim
        h = -(p[i] / rho[i]**2 + p[j] / rho[j]**2) * dwdx[k][d]
        he = he + (vx[j][d] - vx[i][d]) * h
        if @visc
          if d == 0
            h = h + (eta[i] * txx[i] / rho[i]**2 + eta[j] * txx[j] / rho[j]**2) * dwdx[k][0]
            if @dim > 2
              h = h + (eta[i] * txy[i] / rho[i]**2 + eta[j] * txy[j] / rho[j]**2) * dwdx[k][1]
              if @dim == 3
                h = h + (eta[i] * txz[i] / rho[i]**2 + eta[j] * txz[j] / rho[j]**2) * dwdx[k][2]
              end
            end
          elsif d == 1
            h = h + (eta[i] * txy[i] / rho[i]**2 + eta[j] * txy[j] / rho[j]**2) * dwdx[k][0] + (eta[i] * tyy[i] / rho[i] ** 2 + eta[j] * tyy[j] / rho[j]**2) * dwdx[k][1]
            if @dim == 3
              h = h + (eta[i] * tyz[i] / rho[i]**2 + eta[j] * tyz[j] / rho[j]**2) * dwdx[k][2]
            end
          elsif d == 2
            h = h + (eta[i] * txz[i] / rho[i]**2 + eta[j] * txz[j] / rho[j]**2) * dwdx[k][0] + (eta[i] * tyz[i] / rho[i]**2 + eta[j] * tyz[j] / rho[j]**2) * dwdx[k][1] + (eta[i] * tzz[i] / rho[i]**2 + eta[j] * tzz[j] / rho[j]**2) * dwdx[k][2]
          end
        end
        dvxdt[i][d] = dvxdt[i][d] + mass[j] * h
        dvxdt[j][d] = dvxdt[j][d] - mass[i] * h 
      end 
      dedt[i] = dedt[i] + mass[j] * he
      dedt[j] = dedt[j] + mass[i] * he
    end
  end

  for i in 0 ... ntotal
    dedt[i] = tdsdt[i] + 0.5 * dedt[i]
  end  
 
  return [t, c, p, dvxdt, tdsdt, dedt]
end

def art_visc(ntotal, hsml, mass, x, vx, niac, rho, c, pair_i, pair_j, w, dwdx)
  dvx = []
  dvxdt = []
  dedt = []

  alpha = 1.0
  beta = 1.0
  etq = 0.1
  
  for i in 0 ... ntotal
    for d in 0 ... @dim
      dvxdt[i][d] = 0.0
    end
    dedt[i] = 0.0
  end

  for k in 0 ... niac
    i = pair_i[k]
    j = pair_j[k]
    mhsml = (hsml[i] + hsml[j]) / 2.0
    vr = 0.0
    rr = 0.0
    for d in 0 ... @dim
      dvx[d] = vx[i][d] - vx[j][d]
      dx = x[i][d] - x[j][d]
      vr = vr + dvx[d] * dx
      rr = rr + dx * dx
    end

    if vr < 0.0
      muv = mhsml * vr / (rr + mhsml * mhsml * etq * etq)
      mc = 0.5 * (c[i] + c[j])
      mrho = 0.5 * (rho[i] + rho[j])
      piv = (beta * muv - alpha * mc) * muv / mrho
     
      for d in 0 ... @dim
        h = -piv * dwdx[k][d]
        dvxdt[i][d] = dvxdt[i][d] + mass[j] * h
        dvxdt[j][d] = dvxdt[j][d] - mass[i] * h
        dedt[i] = dedt[i] - mass[j] * dvx[d] * h
        dedt[j] = dedt[j] - mass[i] * dvx[d] * h
      end
    end
  end  

  for i in 0 ... ntotal
    dedt[i] = 0.5 * dedt[i]
  end

  return [dvxdt, dedt]
end



def single_step(itimestep, dt, ntotal, hsml, mass, x, vx, u, s, rho, p, t, tdsdt, dx, dvx, du, ds, drho, itype, av)
  pair_i = []
  pair_j = []
  ns = []
  w = []
  dwdx = []
  indvxdt = []
  exdvxdt = []
  ardvxdt = []
  avdudt = []
  ahdudt = []
  c = []
  eta = []
  
  for i in 0 ... ntotal
    avdudt[i] = 0
    ahdudt[i] = 0
    for d in 0 ... @dim
      indvxdt[i][d] = 0
      ardvxdt[i][d] = 0
      exdvxdt[i][d] = 0
    end
  end

  nvirt = 0
  if @virtual_part
    nvirt, hsml, mass, x, vx, rho, u, itype = virt_part(itimestep, ntotal, nvirt, hsml, mass, x, vx, rho, u, p, itype)
  end

  if @nnps == 1
    niac, pair_i, pair_j, w, dwdx, countiac = direct_find(itimestep, ntotal + nvirt, hsml, x)
  end

  if @summation_density
    rho = sum_density(ntotal + nvirt, hsml, mass, niac, pair_i, pair_j, w, itype, rho)
  else
    drho = con_density(ntotal + nvirt, mass, niac, pair_i, pair_j, dwdx, vx)
  end
  
  #Dynamic viscosity
  if @visc
    eta = viscosity(ntotal + nvirt, itype, x, rho, eta) 
  end

  #Internal forces
  t, c, p, dvxdt, tdsdt, dedt = int_force(itimestep, dt, ntotal + nvirt, hsml, mass, vx, niac, rho, eta, pair_i, pair_j, dwdx, u, itype, x, t)

  dvxdt, dedt = art_visc(ntotal + nvirt, hsml, mass, x, vx, niac, rho, c, pair_i, pair_j, w, dwdx) if @visc_artificial

  

  return [rho, p, t, tdsdt, dx, dvx, du, ds, drho, ds, drho, av]
end

def time_integration(x, vx, mass, rho, p, u, itype, hsml, ntotal, maxtimestep, dt)
  x_min = []
  v_min = []
  u_min = []
  rho_min = []
  dx = []
  dvx = []
  du = []
  drho = []
  av = []
  ds = []
  t = []
  tdsdt = []
  
  for i in 0 ... ntotal
    av[i] = []
    for d in 0 ... @dim
      av[i][d] = 0
    end
  end

  nstart = 0
  current_ts = 0
  
  for itimestep in nstart + 1 ... nstart + maxtimestep
    current_ts += 1
    if itimestep % @print_step == 0
      puts "*"*40
      puts "Current number of time step = #{itimestep}"
      puts "*"*40
    end

    if itemstep != 1
      for i in 0 ... ntotal
        u_min[i] = u[i]
        temp_u = 0
        temp_u = -nsym * p[i] * vx[i][0]/x[i][0]/rho[i] if @dim == 1
        u[i] = u[i] + (dt/2.0) * (du[i] + temp_u)
        u[i] = 0 if u[i] < 0
        if !@summation_density
          rho_min[i] = rho[i]
          temp_rho = 0
          temp_rho = -nsym * rho[i] * vx[i][0]/x[i][0] if @dim == 1
          rho[i] = rho[i] + (dt/2.0) * (drho[i] + temp_rho)
        end
        v_min[i] = []
        for d in 0 ... @dim
          v_min[i][d] = vx[i][d] + (dt/2.0) * dvx[i][d]
        end
      end
    end

    rho, p, t, tdsdt, dx, dvx, du, ds, drho, ds, drho, av = single_step(itimestep, dt, ntotal, hsml, mass, x, vx, u, s, rho, p, t, tdsdt, dx, dvx, du, ds, drho, itype, av)

    if itemstep == 1
      for i in 1 ... ntotal
        temp_u = 0
        temp_u = -nsym * p[i] * vx[i][0]/x[i][0]/rho[i] if @dim == 1      
        u[i] = u[i] + (dt/2.0) * (du[i] + temp_u)
        u[i] = 0 if u[i] < 0 
        if !@summation_density
          temp_rho = 0
          temp_rho = -nsym * rho[i] * vx[i][0]/x[i][0] if @dim == 1
          rho[i] = rho[i] + (dt/2.0) * (drho[i] + temp_rho)
        end 
        for d in 0 ... @dim
          vx[i][d] = vx[i][d] + (dt/2.0) * dvx[i][d] + av[i][d]
          x[i][d] = x[i][d] + dt * vx[i][d]
        end
      end  
    else
      temp_u = 0
      temp_u = -nsym * p[i] * vx[i][0]/x[i][0]/rho[i] if @dim == 1      
      u[i] = u_min[i] + dt * (du[i] + temp_u)
      u[i] = 0 if u[i] < 0
      if !@summation_density
        temp_rho = 0
        temp_rho = -nsym * rho[i] * vx[i][0]/x[i][0] if @dim == 1
        rho[i] = rho_min[i] + dt * (drho[i] + temp_rho)
      end
      for d in 0 ... @dim
        vx[i][d] = v_min[i][d] + dt * dvx[i][d] + av[i][d]
        x[i][d] = x[i][d] + dt * vx[i][d]
      end
    end

    #time = time + dt
  #  if itimestep % save_step == 0
      #output(x, vx, mass, rho, p, u, c, itype, hsml, ntotal)
  #  end

    if itimestep % @print_step == 0
      #puts "*"*40
      #puts "Current number of time step = #{itimestep}"
      #puts "*"*40
    end
  end
 
  return [x, vx, rho, p, u, c, s, e, hsml]
end

def sph()
  summation_density = true

  time_print
  
  dt = 0.005 if @shocktube 
  dt = 5.0e-5 if @shearcavity

  maxtimestep = 10

  x, vx, mass, rho, p, u, itype, hsml, ntotal = input()

  x, vx, rho, p, u, c, s, e, hsml = time_integration(x, vx, mass, rho, p, u, itype, hsml, ntotal, maxtimestep, dt)

end

c = [0.3, 0.3, 0.3, 0.3, 0.4]
s = [0.3, 0.3, 0.3, 0.3, 0.4]
e = [0.3, 0.3, 0.3, 0.3, 0.4]

sph()
