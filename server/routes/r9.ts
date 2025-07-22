import getToken from "../components/loadTDXToken";
const url = "https://tdx.transportdata.tw/api/basic/v2/Bus/EstimatedTimeOfArrival/City/Taipei/%E7%B4%859?%24top=1&%24skip=21&%24format=JSON"

const BUS = async () =>{
    const token = await getToken();
    try {
        const api = await fetch(url,
            {
               headers: {
                    Authorization: `Bearer ${token}`,
                    'Content-Encoding': 'gzip',
                },
            }
        );
        const data = await api.json();
        return data;
    } catch (e) {
        console.log(e);
    }
}
export default defineEventHandler(async (event) =>{
    if (event.node.req.method === 'POST' || event.node.req.method === 'GET') {
        try {
            const data = await BUS();
            return data;
        } catch (e) {
            throw createError({
                statusCode: 500,
                statusMessage: `服務無法存取! \n ${e}`
            })
        }
    } else {
        throw createError({
            statusCode: 403,
            statusMessage: '服務無法被存取!'
          })
    }

})