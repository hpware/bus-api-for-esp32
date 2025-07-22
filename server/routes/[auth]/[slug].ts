import getToken from "../../components/loadTDXToken";

// Simple in-memory cache
const cache: Record<string, { data: any; expires: number }> = {};
const CACHE_TTL = 60 * 1000; // 1 minute

const BUS = async (url: string) => {
    // Check cache
    const cached = cache[url];
    const now = Date.now();
    if (cached && cached.expires > now) {
        return cached.data;
    }

    const token = await getToken();
    try {
        const api = await fetch(url, {
            headers: {
                Authorization: `Bearer ${token}`,
                'Content-Encoding': 'gzip',
            },
        });
        const data = await api.json();

        // Store in cache
        cache[url] = {
            data,
            expires: now + CACHE_TTL,
        };

        return data;
    } catch (e) {
        console.log(e);
    }
};

export default defineEventHandler(async (event) => {
    const slug = getRouterParam(event, 'slug'); 
    const auth = getRouterParam(event, 'auth'); 
    if (!slug || !auth) {
        throw createError({
            statusCode: 400,
            statusMessage: 'Slug parameter is required'
        });
    }
    if (auth !== process.env.ACCESS_TOKEN) {
        throw createError({
            statusCode: 401,
            statusMessage: 'Unauthorized access'
        });
    }
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