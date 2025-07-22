import getToken from "../components/loadTDXToken";

const BUS = async (url: string) =>{
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

export default defineEventHandler(async (event) => {
  const slug = getRouterParam(event, 'slug');
  if (event.node.req.method === 'POST' || event.node.req.method === 'GET') {
    try {
      const url = `https://tdx.transportdata.tw/api/advanced/v2/Bus/EstimatedTimeOfArrival/City/Taipei/PassThrough/Station/${"107"}?%24top=30&%24format=JSON`
        const data = await BUS(url);
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
});