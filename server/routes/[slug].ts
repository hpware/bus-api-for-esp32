export default defineEventHandler((event) => {
  const slug = getRouterParam(event, 'slug');
  
  return {
    message: 'Hello, world!',
    timestamp: new Date().toISOString(),
    method: event.req.method,
    url: slug,
  }; 
});
