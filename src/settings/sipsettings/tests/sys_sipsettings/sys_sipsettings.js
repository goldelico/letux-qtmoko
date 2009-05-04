/****************************************************************************
**
** This file is part of the $PACKAGE_NAME$.
**
** Copyright (C) $THISYEAR$ $COMPANY_NAME$.
**
** $QT_EXTENDED_DUAL_LICENSE$
**
****************************************************************************/

//TESTED_COMPONENT=Comms: Internet: VoIP (18727)

include( "accounts.js" );

testcase = {
    initTestCase: function()
    {
        waitForQtopiaStart();
    },

    cleanupTestCase: function()
    {
    },

    init: function()
    {
        startApplication( "security" );
    },

    cleanup: function()
    {
    },

/*
    \req QTOPIA-1873

*/
    connecting_to_a_server_data:
    {
        normalconnection:[Accounts.getAccount(8)]
    },

    connecting_to_a_server: function(sipaccount)
    {
        prompt(
            "* Enter the settings username: "+sipaccount.username+"; server: "+sipaccount.server+"; pass: "+sipaccount.password+"; name "+sipaccount.name+"\n"+
            "* Check the Automatically Connect checkbox.\n"+
            "* Press Done.\n"+
            "* Verify that the user is asked if they wish to connect now.\n"+
            "* Select yes.\n"+
            "* Go to Homescreen.\n"+
            "* Verify that VoIP has connected (VoIP Network Registered message).\n"
        );
    },

/*
    \req QTOPIA-1873
    \groups
*/
    making_and_receiving_voip_call: function()
    {
        prompt(
            "* Precondition: voip_001_phone has passed, a second device (see data) and VoIP user account exists to call.\n"+
            "* Create a Contact with a VoIP number matching the second user account.\n"+
            "* Call the VoIP number.\n"+
            "* Verify that the Calls screen shows with the target displayed as 'Calling'.\n"+
            "* Verify that the second device receives the call notification.\n"+
            "* Accept the call on the second device.\n"+
            "* Verify that the call is connected.\n"+
            "* Speak into the first device.\n"+
            "* Verify that the second device produces the audio clearly.\n"+
            "* Speak into the second device.\n"+
            "* Verify that the first device produces the audio clearly.\n"
        );
    },

/*
    \req QTOPIA-1873
    \groups
*/
    editing_account_settings: function()
    {
        prompt(
            "* Precondition: voip_001_phone has passed.\n"+
            "* View the account options.\n"+
            "* Change the name to Edited, the user id to Unknown.\n"+
            "* Ensure the Connect Automatically option is off.\n"+
            "* Press Done.\n"+
            "* Verify that Qtopia asks if the user wishes to connect.\n"+
            "* Select Yes.\n"+
            "* Return to the Homescreen.\n"+
            "* Verify that the No VoipNetwork message is shown.\n"+
            "* Call a Contact with Voip.\n"+
            "* Verify that the call is rejected (No Voip Network).\n"+
            "* Open VoIP settings and change the user to a valid account.\n"+
            "* Press Done, select Yes.\n"+
            "* Verify that VoIP connects and is available by calling the contact.\n"
        );
    },

/*
    \req QTOPIA-1873
    \groups
*/
    presence_monitor: function()
    {
        prompt(
            "* Precondition: voip_001_phone has passed, a second account has been created and is connected.\n"+
            "* Create a contact with the VoIP contact information of the second account.\n"+
            "* View the contact details.\n"+
            "* Verify the contact icon shows as a green figure (online).\n"+
            "* Disconnect the second user.\n"+
            "* Verify that contact icon shows as a brown figure (offline).\n"+
            "* Press Done.\n"+
            "* Reconnect the second account.\n"+
            "* Open the contact details.\n"+
            "* Verify that the icon shows as a green figure.\n"
        );
    },

/*
    \req QTOPIA-1873
    \groups
*/
    making_a_call_to_a_landline_phone: function()
    {
        prompt(
            "* Precondition: voip_001_phone has passed.\n"+
            "* Ensure that no sim card or phonesim is used (no network).\n"+
            "* Create a contact with the number of the available landline phone.\n"+
            "* View the contact details.\n"+
            "* Select the landline number.\n"+
            "* Verify that a dialog box appears giving the user a choice of GSM or VoIP call.\n"+
            "* Select VoIP.\n"+
            "* Verify that the call screen shows.\n"+
            "* Accept the call.\n"+
            "* Verify that speech can be heard in both directions.\n"+
            "* End the call.\n"+
            "* Restart Qtopia using a phonesim or sim card.\n"+
            "* Repeat the steps 4-11.\n"
        );
    }
}
