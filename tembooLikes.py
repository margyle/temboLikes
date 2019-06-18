from temboo.Library.Instagram import GetMediaByID
from temboo.Library.Instagram import GetRecentMediaForUser
from temboo.core.session import TembooSession
import json,serial, time

#serial address
#you will need to change this to your Arduino's usb connection
#on unix flavored os type "ls /dev/tty*" to find it
ttyAddress = "/dev/tty.usbmodem14311101" 

#consts: fill in you api and temboo info here
accesstoken = ""
sessionAccount = ""
sessionApp = ""
sessionID = ""

# Create a session with your Temboo account details
session = TembooSession(sessionAccount, sessionApp, sessionID)

# Instantiate the ChoreoS

getRecentMediaForUserChoreo = GetRecentMediaForUser(session)
# Create a session with your Temboo account details
print("********************")
# Get an InputSet object for the Choreo
getRecentMediaForUserInputs = getRecentMediaForUserChoreo.new_input_set()

# Set the Choreo inputs
getRecentMediaForUserInputs.set_AccessToken(accesstoken)
#only get the last post = 1
getRecentMediaForUserInputs.set_Count("1")

# Execute the Choreo
getRecentMediaForUserResults = getRecentMediaForUserChoreo.execute_with_results(getRecentMediaForUserInputs)

# Print the Choreo outputs
igResponse = getRecentMediaForUserResults.get_Response()

mostRecent = json.loads(igResponse)
print("Most Recent Post Media ID: " + str(mostRecent["data"][0]["id"]))
print("Total Likes: " + str(mostRecent["data"][0]["likes"]["count"]))

print("********************")

# Get an InputSet object for the Choreo

arduino = serial.Serial(ttyAddress, 19200, timeout=.1)
time.sleep(5) #give the connection a second to settle
arduino.write("<IG," + str(mostRecent["data"][0]["likes"]["count"]) + ">")
while True:
	data = arduino.readline()
	if data:
		print(data.rstrip('\n')) #strip out the new lines for now
		# (better to do .read() in the long run for this reason
