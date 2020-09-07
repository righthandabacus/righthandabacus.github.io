---
layout: post
title: Google Sheet API guided walkthrough
date: Fri, 04 Sep 2020 20:28:38 -0400
tags: code
---

I have been doing this for quite some years and witnessed the
[Google Sheets API](https://developers.google.com/sheets/) became matured. This
are the steps we needed to do from scratch to create a credential and write
some Python code to drive up Google Sheet API.

First is to create a Google API project: On the
[developer console](https://console.developers.google.com/),
we create a new project (such that we can apply access control on project
basis) using the drop down at the left of the top search bar. While in the
project created, click on the "Dashboard" on the sidebar and enable Google
Sheet API.

Then, credential have to be created. Click on "Credentials" on the sidebar and 
create credentials. We need the API key and OAuth 2.0 client ID for this but
Google will also generate service account. The OAuth ID shall be downloaded as
JSON format, `credentials.json`.

Afterwards, we should find the Google Sheet ID. This is the hash key to be used
by the API and the sheet shall be shared for access with the service account.
Then, in Python, we will need
[`gspread`](https://gspread.readthedocs.io/en/latest/) and `oauth2client`
packages for the Sheets API and authentication, respectively. Alternative to
`gspread` would be the `googleapiclient`, which however, is more generic and
officially from Google.

Access using Python is as follows: Using `gspread`:

```python
import gspread
from oauth2client.service_account import ServiceAccountCredentials

credential = ServiceAccountCredentials.from_json_keyfile_name(
    "credentials.json",
    ["https://spreadsheets.google.com/feeds"])
client = gspread.authorize(credentials)
sheet = client.open_by_key("___PutGoogleSheetKeyHere___").sheet1
sheet.append_row(["data", "data", "data", "data"], 1)
```

and using `googleapiclient` will be this:

```python
from googleapiclient.discovery import build
from oauth2client import file, client, tools
from httplib2 import Http

SCOPES = 'https://www.googleapis.com/auth/spreadsheets'

store = file.Storage("token.json")
creds = store.get()
if not creds or creds.invalid:
    flow = client.flow_from_clientsecrets('credentials.json', SCOPES)
    creds = tools.run_flow(flow, store)
service = build('sheets', 'v4', http=creds.authorize(Http()))
sheet = service.spreadsheets()
sheet.values().append(
    spreadsheetId="___PutGoogleSheetKeyHere___",
    body={'values': ['data','data','data','data']},
    valueInputOption="RAW",
    range="'Sheet1'!A1:D"
).execute()
```

Surely, the `gspread` is simpler as many nuts and blots are hidden. Using the
generic API needs to specify the exact API syntax for HTTP REST API
parameters, such as the range have to be correct and matching the dimension of
the value.

Also note that this is for remote access. If we write script (in JavaScript)
inside Google Sheet, we do not need any of these.
