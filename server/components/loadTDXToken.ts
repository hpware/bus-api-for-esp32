// Get Token from TDX
const getTokenURL = "https://tdx.transportdata.tw/auth/realms/TDXConnect/protocol/openid-connect/token";
const clientId = process.env.TDX_CLIENT_ID;
const clientSecret = process.env.TDX_CLIENT_SECRET;
const grantType = "client_credentials";

async function getToken() {
    // If cached token
    //const reqtrackingkey2 = tokenCache.get(reqtrackingkey);

    //if (reqtrackingkey2) {
    //    console.log('Using cached token\n' + reqtrackingkey2);
    //    return reqtrackingkey2;
    //}
    // Pull Token
    const response = await fetch(getTokenURL, {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded",
        },
    body: `client_id=${clientId}&client_secret=${clientSecret}&grant_type=${grantType}`,
    });
    const data = await response.json(); 
    //console.log('Using newly created token\n' + data.access_token);
    //tokenCache.set(reqtrackingkey, data.access_token);
    return data.access_token;
    // The code above must be uncommented in prod
    //return null;
    
}
export default getToken;