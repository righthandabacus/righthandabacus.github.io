#!/usr/bin/env python
# coding: utf-8

import pandas as pd
import numpy as np



# Pandas demo
# -----------
#
# This is to demonstrate the similarity between pandas and SQL. The following
# are the data to be manipulated, obtained from
# [here](https://www.w3schools.com/sql/sql_syntax.asp):

customer_cols = ["CustomerID", "CustomerName", "ContactName", "Address", "City", "PostalCode", "Country"]
customer_data = [
    [1, "Alfreds Futterkiste", "Maria Anders", "Obere Str. 57", "Berlin", "12209", "Germany"],
    [2, "Ana Trujillo Emparedados y helados", "Ana Trujillo", "Avda. de la Constitución 2222", "México D.F.", "05021", "Mexico"],
    [3, "Antonio Moreno Taquería", "Antonio Moreno", "Mataderos 2312", "México D.F.", "05023", "Mexico"],
    [4, "Around the Horn", "Thomas Hardy", "120 Hanover Sq.", "London", "WA1 1DP", "UK"],
    [5, "Berglunds snabbköp", "Christina Berglund", "Berguvsvägen 8", "Luleå", "S-958 22", "Sweden"],
    [89, "White Clover Markets", "Karl Jablonski", "305 - 14th Ave. S. Suite 3B", "Seattle", "98128", "USA"],
    [90, "Wilman Kala", "Matti Karttunen", "Keskuskatu 45", "Helsinki", "21240", "Finland"],
    [91, "Wolski", "Zbyszek", "ul. Filtrowa 68", "Walla", "01-012", "Poland"],
    [92, "Cardinal", "Tom B. Erichsen", "Skagen 21", "Stavanger", "4006", "Norway"],
]
order_cols = ["OrderID", "CustomerID", "EmployeeID", "OrderDate", "ShipperID"]
order_data = [
    [10308, 2, 7, "1996-09-18", 3],
    [10309, 37, 3, "1996-09-19", 1],
    [10310, 77, 8, "1996-09-20", 2],
    [10248, 90, 5, "1996-07-04", 3],
    [10249, 81, 6, "1996-07-05", 1],
    [10250, 34, 4, "1996-07-08", 2],
]
supplier_cols = [
    "SupplierID", "SupplierName", "ContactName", "Address", "City", "PostalCode", "Country"
]
supplier_data = [
    [1, "Exotic Liquid", "Charlotte Cooper", "49 Gilbert St.", "London", "EC1 4SD", "UK"],
    [2, "New Orleans Cajun Delights", "Shelley Burke", "P.O. Box 78934", "New Orleans", "70117", "USA"],
    [3, "Grandma Kelly's Homestead", "Regina Murphy", "707 Oxford Rd.", "Ann Arbor", "48104", "USA"],
    [4, "Tokyo Traders", "Yoshi Nagase", "9-8 Sekimai Musashino-shi", "Tokyo", "100", "Japan"],
]
shipper_cols = ["ShipperID", "ShipperName"]
shipper_data = [
    [1, "Speedy Express"],
    [2, "United Package"],
    [3, "Federal Shipping"],
]
employee_cols = [
    "EmployeeID", "LastName", "FirstName", "BirthDate", "Photo", "Notes"
]
employee_data = [
    [1, "Davolio", "Nancy", "1968-12-08", "EmpID1.pic", "Education includes a BA...."],
    [2, "Fuller", "Andrew", "1952-02-19", "EmpID2.pic", "Andrew received his BTS...."],
    [3, "Leverling", "Janet", "1963-08-30", "EmpID3.pic", "Janet has a BS degree...."],
]
product_cols = [
    "ProductID", "ProductName", "SupplierID", "CategoryID", "Unit", "Price"
]
product_data = [
    [1, "Chais", 1, 1, "10 boxes x 20 bags", 18],
    [2, "Chang", 1, 1, "24 - 12 oz bottles", 19],
    [3, "Aniseed Syrup", 1, 2, "12 - 550 ml bottles", 10],
    [4, "Chef Anton's Cajun Seasoning", 2, 2, "48 - 6 oz jars", 22],
    [5, "Chef Anton's Gumbo Mix", 2, 2, "36 boxes", 21.35],
]
orderdetail_cols = [
    "OrderDetailID", "OrderID", "ProductID", "Quantity"
]
orderdetail_data = [
    [1, 10248, 11, 12],
    [2, 10248, 42, 10],
    [3, 10248, 72, 5],
    [4, 10249, 14, 9],
    [5, 10249, 51, 40],
]



# Create
# ------
#
# SQL use `CREATE TABLE` and `INSERT INTO` to build a data table. Pandas can use
# `read_csv()` or `read_excel()` to load data from readily-available sources,
# for example (URL can be replaced with file path):

url = "https://research.stlouisfed.org/fred2/series/UNRATE/downloaddata/UNRATE.csv"
df_csv = pd.read_csv(url, parse_dates=True)

url = "http://api.worldbank.org/v2/en/indicator/gc.dod.totl.gd.zs?downloadformat=excel"
df_xl = pd.read_excel(url, sheet_name='Data', skiprows=3)

# or we can create the data manually. For example, and empty DataFrame is
# defined by providing only the column names:
df_empty = pd.DataFrame(columns=customer_cols)

# and we can provide rows of data directly (matching the columns) to create a
# populated DataFrame:
df = pd.DataFrame(customer_data, columns=customer_cols)

# We can also provide data in Python dict, either as list of dict, each dict
# correspond to one row, or dict of list, each correspond to one column. The
# DataFrame will have the columns in unspecified order, which we need to use
# `filter()` to enforce an order afterwards:
lst_of_dict = [dict(zip(customer_cols, row)) for row in customer_data]
df_lod = pd.DataFrame(lst_of_dict).filter(customer_cols)

dict_of_lst = {col: [row[i] for row in customer_data] for i, col in enumerate(customer_cols)}
df_dol = pd.DataFrame(dict_of_lst).filter(customer_cols)

# We can also use a list of named tuples rather than list of dict to present
# data to pandas:
import collections
record = collections.namedtuple("record", customer_cols)
lst_of_ntup = [record(*row) for row in customer_data]
df_ntup = pd.DataFrame(lst_of_ntup)

# sometimes we use random data to test out features. Below is an example to use
# numpy to generate standard normal distributed numbers of 100 rows and 4
# columns:
df_random = pd.DataFrame(np.random.randn(100, 4), columns=list('ABCD'))



# Select
# ------
#
# SQL use `SELECT * FROM table` to show all data from a table. Printing pandas
# DataFrame is similar to that. Besides, we can convert data from a DataFrame to
# other formats. For example, convert to list of dicts, either using
# `to_dict("records")` or constructing each row of data in a loop:
lst_of_dict = df.to_dict("records")

lst_of_dict = [row.to_dict() for _, row in df.iterrows()]

lst_of_dict = [dict(zip(df.columns, row)) for row in df.values]

# We can also extract the underlying numpy array of a DataFrame:
df.values

# The SQL statement `SELECT column FROM table` (i.e., cover all rows but some
# subset of columns) is equivalent to the following in pandas. The `copy()` is a
# safety precaution that the extracted data will become independent of the
# original one so that it can be manipulated:
df2 = df[["CustomerName", "ContactName"]].copy()

# Instead of using a selector, we can also use `filter()` function for the same
# purpose. Only difference is that `filter()` will silently ignore those columns
# that do not exist:
df2 = df.filter(["CustomerName", "ContactName", "foo"]).copy()



# Count and distinct
# ------------------
#
# SQL can remove redundant rows of data by
# `SELECT DISTINCT column1, column2 FROM table_name`.
# Pandas can do similar column selection and then apply `drop_duplicates()`:
df2 = df.filter(["CustomerID", "CustomerName"]).drop_duplicates()

# Similarly, a count of distinct can also be done. SQL will be
# `SELECT COUNT(DISTINCT Country) FROM Customers`
# while pandas can use `len()` function:
count = len(df.filter(["Country"]).drop_duplicates())

# The full dimension of a DataFrame is provided by `.shape`, which counts the
# total number of rows and columns in a tuple:
assert df.shape == (len(df), len(df.columns)))

# But if we count only the non-null cells, we use `count()`. For example, this
# counts the number of rows with non-null country:
df["Country"].count()

# and this counts the *number of cells* in the whole DataFrame that are not null:
df.count()



# Select rows
# -----------
#
# SQL has the `LIMIT` clause to control the amount of output. Pandas counterpart
# is the `iloc[]` and `loc[]` slicers. For example, SQL statement
# `SELECT * FROM Customers LIMIT 3` becomes the following, for slicing rows
# without referencing to the row indices:
df_subset = df.iloc[:3]

# or using `loc[]` to slice by row indices (row ids of some DBMS):
df_subset = df.loc[[1,3,5]]

# or just investigate the first or last few rows of a DataFrame:
df_subset = df.head(4)
df_subset = df.tail(4)

# The `iloc[]` and `loc[]` can indeed filter by column as well. Similar to SQL
# statement
# `SELECT column1, column2 FROM Customers LIMIT 3 OFFSET 1`
# `iloc[]` requires both row and column are specified as offset ranges while
# `loc[]` requires they are specified as indices/labels. Such as the following:
df_subset = df.iloc[2:5, 0:2]
df_subset = df.loc[[2,3,4], ["CustomerID", "CustomerName"]]
df_subset = df.loc[2:4, "CustomerID":"CustomerName"]

# Row indices and column labels of a DataFrame are provided respectively by
df.index
df.columns

# Both `iloc[]` and `loc[]` accept slicing by enumerated list or a range. But in
# `iloc[]` the range is interpreted as if a Python range which the upperbound is
# not included in the output. However, `loc[]` will include both ends of the
# range.
#
# SQL has `WHERE` clause to select rows based on some boolean criteria. In
# pandas, we select using "boolean indexing", i.e., by providing a boolean
# vector of size as same number of rows of the DataFrame. Expressions that we
# can use in `WHERE` clause may contain operators such as `=`, `<>`, `<`, `>`,
# `>=`, `<=`, `BETWEEN`, `LIKE`, `IN`, `AND`, `OR`, `NOT`, `IS NULL`, and
# `IS NOT NULL`. Similar operation also exists in pandas:
boolindex = (df["Country"] == "Mexico") | df["City"].str.startswith("S") | (df["CustomerId"] < 50)
df_subset = df[boolindex]

# `SELECT * FROM Customers WHERE Country='Germany' AND (City='Berlin' OR # City='München')`
# becomes
df_subset = df[df["Country"].eq("Germany") & df["City"].isin(["Berlin", "München"])]

# Using IS NULL: `SELECT CustomerName, ContactName, Address FROM Customers WHERE Address IS NULL`
df_subset = df[df["Address"].isna()].filter(["CustomerName", "ContactName", "Address"])

# Pandas provide boolean operators `&` (and), `|` (or), `~` (not) and functions
# `.gt(x)`, `.ge(x)`, `.lt(x)`, `.le(x)`, `.eq(x)`, `ne(x)`, `notna()`,
# `isna()`. String functions are available as `.str.*`. Simple boolean is
# further supported by `query()`:
df_subset = df.query("CustomerID % 2 == 0")

# Nested query can also be used intuitively, e.g.
# `SELECT * FROM Customers WHERE Country IN (SELECT Country FROM Suppliers)`
# becomes:
df_subset = df[df["Country"].isin(df_suppliers["Country"])]



# Expression on output
# --------------------
#
# SQL can provide expression as `SELECT` result column. Pandas need to create
# such result column as additional columns. For example
# `SELECT CustomerID, CustomerID^2 AS "ID2" FROM Customer` is equivalent to:
df_result = df[["CustomerID"]]
df_result["ID2"] = df["CustomerID"] ** 2

# or we can combine it as one line:
df_result = df[["CustomerID"]].assign(ID2=df["CustomerID"]**2)

# Such simple expression also supported by `eval()`
df["ID2"] = df.eval("CustomerID ** 2")

# Alternatively, we can call `eval()` in-place instead of using assignment:
df.eval("ID2 = CustomerID ** 2", inplace=True)

# More complicated expression have to be defined as a function or lambda that
# return a row and `apply()` on the DataFrame on row-by-row basis.
def names(row):
    tokens = row["CustomerName"].split()
    row["FirstName"] = tokens[0]
    row["LastName"] = tokens[-1]
    return row
df_result = df.apply(names, axis=1).filter(["FirstName", "LastName"])

# The result of `DataFrame.apply()` is a DataFrame if the function returns a
# Series, and it is a Series if the function returns a scalar. If the function
# modifies the row, as above, the change is in-place to the DataFrame.
#
# We also have functions to modify a column (Series) directly, e.g.
df["Customer"] = df["CustomerName"].str.upper().apply(lambda n: n.split()[-1]) + \
                 " " + df["CustomerName"].apply(lambda n: n.split()[0])

# Similar to the case of evaluting new result column, we can assign a new column
# by providing a series directly, as long as the list has same length as the
# number of  rows in the DataFrame:
df["foo"] = [1, 2, 3, 4]



# Case statements
# ---------------
#
# Besides using a function to `apply()`, we can mimick case statements by a
# series of `mask()` or `where()`. For example:
# ```SELECT OrderID, Quantity,
# CASE
#     WHEN Quantity > 30 THEN "The quantity is greater than 30"
#     WHEN Quantity = 30 THEN "The quantity is 30"
#     ELSE "The quantity is under 30"
# END AS QuantityText
# FROM OrderDetails```
# becomes
df_order["QuantityText"] = "The quantity is under 30"
df_order["QuantityText"] = df_order["QuantityText"] \
                           .mask(df_order["Quantity"].eq(30), "The quantity is 30") \
                           .mask(df_order["Quantity"].gt(30), "The quantity is greater than 30")
df_result = df_order[["OrderID", "Quantity", "QuantityText"]]

# `mask()` and `where()` has the same syntax that it takes a boolean mask and a
# data to replace to. Only that `mask()` replace the original value whenever the
# boolean is True while `where()` replace whenever the boolean is False. The
# condition should be specified in reverse order as `CASE` statement because
# value is taken as last-match-wins while the `CASE` statement is
# first-match-wins.



# Aggregate functions
# -------------------
#
# Aggregate functions allows us to find the sum, mean, min, max, and so on. SQL
# aggregate functions are provided as DataFrame or Series methods in pandas,
# e.g. `SELECT MIN(Price) AS SmallestPrice FROM Products`
# becomes
smallest_price = df["Products"]["Price"].min()

# `SELECT SUM(Quantity) FROM OrderDetail WHERE OrderID=10248`
# becomes:
df = pd.DataFrame(orderdetail_data, columns=orderdetail_cols)
total = df[df["OrderID"] == 10248]["Quantity"].sum()

# And we can also use `sum()` as count function when applied to boolean index,
# for example,
# `SELECT COUNT() FROM OrderDetail WHERE Quantity > 10`
# becomes:
count = df["Quantity"].gt(10).sum()



# Ordering
# --------
#
# Pandas DataFrame has `sort_values()` to correspond to the ORDER clause in SQL.
# Example:
df_sorted = df.sort_values("Country", ascending=True)

# Another example, `SELECT * FROM Customers ORDER BY Country ASC, CustomerName DESC`
# becomes
df_sorted = df.sort_values(["Country", "CustomerName"], ascending=[True, False])

# If we have to sort by a function, we can use a temporary column
df.assign(f = df['one']**2 + df['two']**2).sort_values('f').drop('f', axis=1)

# Besides sort by value, we can also sort by index. This is useful if we have a
# row index (axis=0), or want to sort the column by string order (axis=1). For example:
df.sort_index(axis=0, ascending=False)



# Append and insert
# -----------------
#
# SQL has UNION to combine tables. Pandas does that with `concat()`:
df_combined = pd.concat([df1, df2])

# or using `append()`
df_new = df.append(df2, sort=False)

# another example of `append()`: in SQL
# `SELECT City FROM Customers UNION SELECT City FROM Suppliers ORDER BY City`
# becomes
cities = df_customers["Country"].append(df_suppliers["Country"]).sort_values()

# Besides, we can also append a row or DataFrame to an existing DataFrame:
df_new = df.append({}, ignore_index=True)

# Or, add a row in-place by providing a new index,
df.loc[101] = [99, np.nan, n.nan, np.nan, np.nan, 98765, "USA"]

# If we insert new row to the DataFrame but not every columns are provided, we
# can use the `.loc[]` slicing. For example the SQL:
# ```INSERT INTO Customers (CustomerName, ContactName, Address, City, PostalCode, Country)
# VALUES ('Cardinal', 'Tom B. Erichsen', 'Skagen 21', 'Stavanger', '4006', 'Norway')```
# becomes
df.loc[101, ["CustomerName", "ContactName", "Address", "City", "PostalCode", "Country"]] = \
    ['Cardinal', 'Tom B. Erichsen', 'Skagen 21', 'Stavanger', '4006', 'Norway']



# Casting
# -------
#
# We can change the data type of a column by `CAST()` in SQL and by `astype()`
# in pandas DataFrame:
df["CustomerID"] = df["CustomerID"].astype("int")

# In case we do not need conversion like str to int, we can ask pandas to infer
# the type of each column automatically:
df = df.infer_objects()

# Special case is the datatime object: Pandas do not use Python datetime object
# but carries its own version so that "not a time" can be defined. To convert
# the pandas datetime64 into Python datetime or UNIX timestamp, we use:
dt64.to_pydatetime()
dt64.timestamp()


# Update data
# -----------
#
# Update a particular cell: We specify the row index and column, such as
# `UPDATE Customer SET City='Seattle' WHERE rowid=4`
df.loc[4].City = 'Seattle'

# We can also using a condition to select the rows, e.g. in SQL
# `UPDATE Customers SET ContactName='Alfred Schmidt', City='Frankfurt' WHERE CustomerID=1`
# becomes
df.loc[df["CustomerID"]==1, ["City", "ContactName"]] = ["Frankfurt", "Alfred Schmidt"]

# Similar construct can be used to update columns by an expression, e.g.
# `UPDATE df SET B=B*C WHERE A>10`
# becomes
selector = df["A"] > 10
df.loc[selector, "B"] *= df[selector]["C"]

# Above, we cannot write as `df.loc[selector]["B"] *= df[selector]["C"]` because the
# result of row slicing is a new DataFrame and the update is not propagated back.
#
# Pandas has a simpler interface to replace all cells by a mapping table. For
# example, replace all empty string `""` into `None`:
df = df.replace({"":None})

# or if the cell the replace are all the "nan" cells, we have `fillna()` (but
# not to replace to None):
df = df.fillna("foo")



# Update columns and delete data
# ------------------------------
#
# To rename a column, SQL has `ALTER TABLE` command. In pandas, we provide a
# dict as name mappings and using `rename()` function:
df.rename(columns={"CustomerID": "ID"})

# To delete columns from a DataFrame, we use `drop()`:
df_trimed = df.drop("Country", axis=1)
df_trimed = df.drop(["Country", "PostalCode"], axis=1)

# Deleting rows is logically same as SELECT those rows to retain. For example,
# select no rows but retain the column structure:
df_empty = df.iloc[0:0]
df_empty = df.loc[0:-1]

# We can also remove rows by row index (default `axis` argument to be 0):
df_trimed = df.drop([0, 1])

# If delete is conditioned on certain rows with nan (default to all rows), we
# have a simpler function:
df_trimed = df.dropna(subset=["Country", "City"])



# Table joins
# -----------
#
# SQL table join is `merge()` in pandas DataFrame. For example:
# ```SELECT o.OrderID, o.OrderDate, c.CustomerName
# FROM Customers AS c, Orders AS o
# WHERE c.CustomerName="Around the Horn" AND c.CustomerID=o.CustomerID```
# becomes
df_result = df_customer \
            .merge(df_orders, how="inner", on="CustomerID") \
            .pipe(lambda df: df["CustomerName"]=="Around the Horn") \
            .filter(["OrderID", "OrderDate", "CustomerName"])

# which the `merge()` can specify `how` to be any of `"inner"`, `"left"`,
# `"right"`, or `"outer"`.
#
# If instead of join by column we want to join by row index, the function is `join()`
df_result = df1.join(df2)



# Group by
# --------
#
# SQL `GROUP BY` is retained in pandas. For example, reading the first row of
# each group (`SELECT DISTINCT ON` in PostgreSQL):
df.groupby("id", as_index=False).first()

# and the SQL
# `SELECT COUNT(CustomerID), Country FROM Customers GROUP BY Country ORDER BY COUNT(CustomerID) DESC`
# becomes
df_result =  df.filter(["Country", "CustomerID"]) \
               .groupby("Country", as_index=False) \
               .count() \
               .sort_values("CustomerID", ascending=False)

# Mostly we use `groupby()` for more complicated calculations. For example, the
# weighted average, by way of numpy:
df_wac = df_orders.groupby(["Deal"]) \
         .apply(lambda grp: np.average(grp["Coupon"], weights=grp["CurrBal"])) \
         .rename("WAC") \
         .reset_index()

# Take the most common (i.e., mode) term can similarly using `mode()` of a row group:
df_mode = df[[col1, col2, col3, col4]].groupby(col1).agg(lambda x: x.mode().iloc[0])

# The `HAVING` clause in SQL is simply additional row slicing after
# `groupby()`. For example,
# ```SELECT Employees.LastName, COUNT(Orders.OrderID) AS NumberOfOrders
# FROM Orders
# INNER JOIN Employees ON Orders.EmployeeID = Employees.EmployeeID
# WHERE LastName = 'Davolio' OR LastName = 'Fuller'
# GROUP BY LastName
# HAVING COUNT(Orders.OrderID) > 25```
# becomes
df_result = df_orders.merge(df_empl, how="inner", on="EmployeeID") \
            .pipe(lambda df: df[df["LastName"].isin(["Davolio", "Fuller"])]) \
            .groupby("LastName") \
            .rename("NumberOfOrders") \
            .reset_index() \
            .pipe(lambda df: df[df["NumberOfOrders"] > 25])

# By default, `groupby()` will make the group value as row index unless we
# specify `as_index=False`. We do `reset_index()` at the end to make index back
# to an ordinary column (hence convert a Series into a two-column DataFrame).
#
# The GroupBy object has a different set of member functions. For example, we
# use `size()` to produce a equiv for
# `SELECT Country, COUNT(*) AS Counts FROM Customers GROUP BY Country`
df_count = df.groupby('Country').size().reset_index(name='Counts')

# The above `reset_index()` is called with a `name` argument, which assigns the
# column name to the result of `size()` in addition to making the group-by index
# as a column. This is useful in case of different aggregate functions to be
# called in the same group-by, e.g., in SQL
# `SELECT Country, SUM(Quantity) AS TotalQty, COUNT(*) AS Count FROM Product # GROUP BY Country`
# becomes
groups = df_product.groupby("Country", as_index=False)
df_result = groups \
            .sum()[["Country", "Quantity"]].rename(columns={"Quantity":"TotalQty"}) \
            .merge(groups.size().reset_index(name="Count"), on="Country")



# Pivot table
# -----------
#
# Pivot table is something cannot be done in SQL but easily achieved in
# spreadsheet. For example, we start with a table of data
data = [ln.split() for ln in """
1  1    0.10    0.22     0.55     0.77
2  1    0.20    0.12     0.99     0.125
1  2    0.13    0.15     0.15     0.245
2  2    0.33    0.1      0.888    0.64
""".strip().splitlines()]
cols = "A  B  01-2016  02-2017  03-2017  04-2020".split()
df = pd.DataFrame(data, columns=cols).astype("float64")
df[["A","B"]] = df[["A","B"]].astype("int")

# now the DataFrame looks like
#
#     >>> df
#        A  B 01-2016 02-2017 03-2017 04-2020
#     0  1  1    0.10    0.22    0.55    0.77
#     1  2  1    0.20    0.12    0.99   0.125
#     2  1  2    0.13    0.15    0.15   0.245
#     3  2  2    0.33     0.1   0.888    0.64
#
# and we `melt()` the DataFrame to combine all dates columns into one:
df_melted = df.melt(id_vars=["A", "B"], var_name="date")
df_melted["value"] = df_melted["value"].astype("float64")

# which the DataFrame produced is like
#
#     >>> df.melted(id_vars=["A","B"], var_name="date")
#         A  B     date  value
#     0   1  1  01-2016   0.10
#     1   2  1  01-2016   0.20
#     2   1  2  01-2016   0.13
#     3   2  2  01-2016   0.33
#     4   1  1  02-2017   0.22
#     5   2  1  02-2017   0.12
#     6   1  2  02-2017   0.15
#     7   2  2  02-2017    0.1
#     8   1  1  03-2017   0.55
#     9   2  1  03-2017   0.99
#     10  1  2  03-2017   0.15
#     11  2  2  03-2017  0.888
#     12  1  1  04-2020   0.77
#     13  2  1  04-2020  0.125
#     14  1  2  04-2020  0.245
#     15  2  2  04-2020   0.64
#
# and to reverse, we use `pivot_table()`:
df_pivot = df_melted \
           .pivot_table(values="value", index="date", columns=["A", "B"]).T \
           .reset_index()

# and this will becomes
#
#     >>> df_pivot
#     date  A  B  01-2016  02-2017  03-2017  04-2020
#     0     1  1     0.10     0.22    0.550    0.770
#     1     1  2     0.13     0.15    0.150    0.245
#     2     2  1     0.20     0.12    0.990    0.125
#     3     2  2     0.33     0.10    0.888    0.640
#
# There is a `pivot()` function besides `pivot_table()` but the former only
# allow a single column to be the `index` or `columns` argument.



# Other algorithmic usages
# ------------------------
# Convert a DataFrame into iterable of rows with row index using `iterrows()`:
for i, row in df_random.iterrows():
    row["A"] = row["A"] * 2

# export data into external format: To Excel and to CSV, or even to string using
# `tabulate` module in Python:
with pd.ExcelWriter(filename, engine="openpyxl") as writer:
    df.to_excel(writer, sheetname, index=False)
df.to_csv(filename)
print(tabulate.tabulate(df, headers="keys", show_index=False))

# Export to HTML can be more complicated:
html = df.to_html(na_rep='N/A', index=False, justify='center', float_format=lambda f: "{0:,.2f}".format(f))

numcols = df.select_dtypes(include=[np.number]).columns
html = df.style.set_properties(subset=numcols, **{'text-align':'right'}) \
         .set_uuid("order") \
         .format(lambda v: v if v==v else '') \
         .format({col: lambda v:"{0:,.2f}".format(v) if v==v else ''
                  for col in ret.select_dtypes(include=[np.float]).columns}) \
         .render()

# To investigate the column types of a DataFrame:
df.dtype

# Pandas has `squeeze()` function to lower the dimension: It will produce a
# series from DataFrame if there is only one row, or retain as DataFrame
# otherwise.
df_or_series = df[df["location"] == "c"].squeeze()

# If the DataFrame is large and we want to parallelize it, we can build a few
# rows at a time in separate processes, using a process pool:
pool = multiprocessing.Pool(4)
df = pd.concat(pool.map(func, list_of_scalar))
df = pd.concat(pool.starmap(func, list_of_tuples))

# but the above will need the function to be defined at module level and
# pickle-able in order to send to difference processes.
#
# If we want a mapping to a column's value, we can prepare a dict of the
# mapping, and apply the dict's `get()` function to the column Series. Pandas
# provides the `map()` function for the same:
mapping_series = df_customer.filter(["CustomerID", "CustomerName"]).set_index("CustomerID")["CustomerName"]
df_order["CustomerName"] = df_order["CustomerID"].map(mapping_series)

# Series has a different set of member functions. For example, combining two series:
s1 = pd.Series({'foo':1, 'bar':2})
s2 = s1.combine_first(pd.Series({'fubar':3}))

# or converting a series into list:
assert s1.tolist() == list(s1)


# Terminology
# -----------
#
# Panel (3D), DataFrame (3D), Series (1D)
# - Series index: row id of each item in vector
# - DataFrame index: row id (numeric from 0 onward by default)
# - DataFrame column: column id
# - Concat series into dataframe:
#   - by axis 0 = output in vector
#   - by axis 1 = concat column vectors horizontally into a table
# - Concat dataframes into another dataframe:
#   - by axis 0 = stacking vertically
#   - by axis 1 = stacking horizontally



# Data Sources
# ------------
#
# Following are some handy source of tabular data that can be used as test data
# sets to try out pandas functions:
#
# - Trading economics: <https://tradingeconomics.com/hong-kong/indicators>
# - [HKMA market data and statistics](https://www.hkma.gov.hk/eng/market-data-and-statistics/economic-and-financial-data-for-hong-kong.shtml)
# - St. Louis Fed time series: <https://fred.stlouisfed.org/>
# - The World Bank open data: <https://data.worldbank.org/>



# vim:set et ts=4 sw=4 nowrap:
