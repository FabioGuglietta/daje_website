"""Visualize the trajectory written by the planetary exercise."""
import argparse
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np


def read_trajectory(path):
    data = np.loadtxt(path, comments='#', ndmin=2)
    if data.shape[1] != 6 or not np.isfinite(data).all():
        raise ValueError('Expected finite columns: time id x y vx vy.')
    if not np.equal(data[:, 1], np.floor(data[:, 1])).all():
        raise ValueError('Body identifiers must be integers.')
    times = np.unique(data[:, 0])
    ids = np.unique(data[:, 1]).astype(int)
    ordered = data[np.lexsort((data[:, 1], data[:, 0]))]
    if len(data) != len(times) * len(ids):
        raise ValueError('Each saved time must contain every body exactly once.')
    records = ordered.reshape(len(times), len(ids), 6)
    if not np.array_equal(records[:, :, 0], np.repeat(times[:, None], len(ids), axis=1)):
        raise ValueError('Each saved time must contain every body exactly once.')
    if not np.array_equal(records[:, :, 1], np.repeat(ids[None, :], len(times), axis=0)):
        raise ValueError('Missing or duplicate body identifiers at a saved time.')
    return times, ids, records[:, :, 2:4]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('trajectory', type=Path)
    parser.add_argument('--animate', action='store_true')
    parser.add_argument('--save', type=Path, help='Save a static orbit plot, e.g. orbits.png.')
    parser.add_argument('--no-show', action='store_true', help='Save figures without opening windows.')
    args = parser.parse_args()
    if args.animate and args.no_show:
        parser.error('--animate requires an interactive window; remove --no-show.')
    try:
        times, ids, positions = read_trajectory(args.trajectory)
    except (OSError, ValueError) as error:
        parser.exit(1, f'Cannot read simulation output: {error}\n')

    fig, ax = plt.subplots(figsize=(7, 7), layout='constrained')
    lines, dots = [], []
    for j, identifier in enumerate(ids):
        line, = ax.plot(positions[:, j, 0], positions[:, j, 1], label=f'Body {identifier}')
        dot, = ax.plot([positions[-1, j, 0]], [positions[-1, j, 1]], 'o', color=line.get_color())
        lines.append(line)
        dots.append(dot)
    ax.set(xlabel='x [reduced units]', ylabel='y [reduced units]', title='Planetary trajectories')
    ax.set_aspect('equal', adjustable='box')
    ax.grid(alpha=0.3)
    ax.legend()
    if args.save:
        fig.savefig(args.save, dpi=160)
    animation = None
    if args.animate:
        ax.set_xlim(ax.get_xlim())
        ax.set_ylim(ax.get_ylim())
        frame_indices = np.unique(np.linspace(0, len(times) - 1, min(800, len(times))).astype(int))

        def update(k):
            for j, (line, dot) in enumerate(zip(lines, dots)):
                line.set_data(positions[:k + 1, j, 0], positions[:k + 1, j, 1])
                dot.set_data([positions[k, j, 0]], [positions[k, j, 1]])
            ax.set_title(f'Time = {times[k]:.3f}')
            return lines + dots

        animation = FuncAnimation(fig, update, frames=frame_indices, interval=30, repeat=True, blit=False)
    if not args.no_show:
        plt.show()
    # Keep the animation alive until the interactive window has closed.
    if animation is not None:
        del animation
    plt.close('all')


if __name__ == '__main__':
    main()
