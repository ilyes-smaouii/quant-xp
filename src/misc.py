from matplotlib import pyplot as plt
import numpy as np
import math as m


def get_normal():
  """
  Generate random real number sampled from standard normal distribution
  """
  return np.random.normal()


def generateStockPrices(initial_price, drift, sigma, timestep=.0001, data_count=100):
  """
  initial_price : initial price
  drift : []
  sigma : volatility

  all variables are annualized
  """
  curr_ts = 0
  curr_price = initial_price
  timestamps = [curr_ts]
  prices = [curr_price]
  for i in range(data_count):
    curr_ts += timestep
    curr_price *= m.exp(timestep * (drift - 0.5 * sigma *
                        sigma) + sigma * np.sqrt(timestep) * get_normal())
    timestamps.append(curr_ts)
    prices.append(curr_price)
  return (timestamps, prices)


def plotStocks(stocks):
  for stock_x, stock_y in stocks:
    plt.plot(stock_x, stock_y)


def pltFullScreen():
  manager = plt.get_current_fig_manager()
  manager.resize(1920, 1080)


if __name__ == "__main__":
  stocks = []
  stock_count = 100
  drift = 0.07
  volatility = 0.20

  for i in range(stock_count):
    stocks.append(generateStockPrices(
      100, drift, volatility, timestep=.001, data_count=3000))

  plotStocks(stocks)
  pltFullScreen()
  plt.show()
