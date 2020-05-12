# pi

A small program to generate the digits of pi, using the fact that

```
pi = pi + sin(pi)
```

Usage: `./pi` - generate 100 digits of pi; `./pi n` - generate n digits of pi

Takes time roughly proportional to n<sup>2</sup> to generate n digits.
