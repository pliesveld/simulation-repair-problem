import sys
import pandas as pd
import numpy as np


def main_app():
    k = 10000
    T = np.empty(k)
    T.fill(0)

    for t in range(0, k):
        T[t] = main_sim()

    ser = pd.Series(T)
    print("E[T] = %f" % ser.mean())


def main_sim(n=10, s=5):

    def G_gen():
        while True:
            yield np.random.standard_exponential()

    def F_gen():
        while True:
            yield np.random.standard_exponential()

    def G():
        return next(G_gen())

    def F():
        return next(F_gen())

    # initialization
    t = r = 0
    t_star = sys.float_info.max

    # Timing function
    event_list = [F() for x in range(0, n)]
    event_list.sort()

    # print(event_list)
    # print("n: %d" % n)
    # print("s: %d" % s)

    while True:
        # print("  r: %d" % r)
        # print("  t: %f" % t)
        # print("  t_star: %f" % t_star)
        # print("  t_n: %f" % event_list[0])
        # check event type i

        if event_list[0] < t_star:

            # working machine breaks down
            t = event_list.pop(0)

            # print("t @ %f " % t, end='')
            # print("Working machine broke down")

            r = r + 1
            if r == s + 1:
                # print("Simulation Over. T = %f" % t)
                return t

            if r < s + 1:
                X = F()
                event_list.append(t + X)
                event_list.sort()

            if r == 1:
                # Immediately start repairing broken machine
                Y = G()
                t_star = t + Y
        else:
            t = t_star
            # print("t @ %f " % t, end='')
            # print("Repair completed")
            r = r - 1

            if r > 0:
                Y = G()
                t_star = t + Y

            if r == 0:
                t_star = sys.float_info.max

if __name__ == '__main__':
    main_app()

