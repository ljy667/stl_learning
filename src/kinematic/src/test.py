from scipy import optimize
import numpy as np
import M20id35_CarDist as cd
import matplotlib.pyplot as plt

# 测试
tcp_offset = [0, 0, 350, 0, 0, 0]
xyzwpr = [1393.418981551881, -1114.7791193709695, 4.498948661275319, 175.22900027549804, -3.2019035289472817, -91.18153706476676]

def car_dist_func( xyzwpr, tcp_offset = [0,0,0,0,0,0], x_min=-1000.0, x_max=800.0, step=100, eps=30):
    def car_dist_inner(deltax):
        xyzwpr0 = xyzwpr.copy()    
        xyzwpr0[0] -= deltax  # 小车移动对应x轴偏移
        return cd.car_dist(xyzwpr0, tcp_offset=tcp_offset)  # 需确保car_dist函数已实现（机械臂可达性计算）

    def find_min_x_positive(car_dist_in, x_min=-800.0, x_max=800.0, step=100, eps=30):
        if car_dist_in(x_min) <= 0 and car_dist_in(x_max) >= 0:
            xs = np.arange(x_min, x_max, step)
            # 找到跳跃区间
            y1 = car_dist_in(xs[0])
            for i in range(1, len(xs)):
                y2 = car_dist_in(xs[i])
                if y1 <= 0 and y2 > 0:
                    x0 = optimize.brentq(car_dist_in, xs[i-1], xs[i])
                    break
                else:
                    y1 = y2
            return x0 + eps
        
        elif car_dist_in(x_max) < 0:    
            return 100000
        
        elif car_dist_in(x_min) > 0:
            return 0
    
    min_x = find_min_x_positive(car_dist_inner, x_min=x_min, x_max=x_max, step=step, eps=eps)
    return min_x


# 初始化用于可视化的数据列表
positions = []
delta_cpp_vals = []
delta_x_vals = []

for x in range(1600, 500, -10):
    xyzwpr[0] = x
    delta_x = car_dist_func(xyzwpr, tcp_offset)

    print(f"抓取位置是：{xyzwpr}，delta: {delta_x}")

    delta_cpp = cd.car_dist(xyzwpr , tcp_offset)

    # 收集数据
    positions.append(xyzwpr[0])
    delta_cpp_vals.append(delta_cpp)
    delta_x_vals.append(delta_x)

    #print(f"抓取位置是：{xyzwpr}，delta: {delta_cpp}")


if positions:
    plt.figure(figsize=(9,4))
    plt.plot(positions, delta_cpp_vals, '-o', label='delta_cpp')
    #plt.plot(positions, delta_x_vals, '-x', label='delta_x')
    plt.xlabel('x')
    plt.ylabel('delta')
    plt.title('delta_cpp and delta_x vs x')
    plt.legend()
    plt.grid(True)
    out_path = 'delta_cpp_vs_x.png'
    plt.savefig(out_path, dpi=200)
    print(f"Saved plot to {out_path}")
    try:
        plt.show()
    except Exception:
        pass
    