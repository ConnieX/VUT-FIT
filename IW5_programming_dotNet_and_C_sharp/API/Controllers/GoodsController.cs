using System.Linq;
using System.Threading.Tasks;
using API.Data;
using API.Models.GoodsModels;
using AutoMapper;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using NSwag.Annotations;

namespace API.Controllers {
    [ApiController]
    [Route("api/[controller]")]
    [ApiVersion("1.0")]
    public class GoodsController : ControllerBase {
        private readonly ApplicationDbContext _database;
        private IMapper _mapper;


        public GoodsController(ApplicationDbContext database, IMapper mapper) {
            _mapper = mapper;
            _database = database;
        }


        [HttpGet]
        public IActionResult GetAll() {
            return new JsonResult(_database.Goods.Select(x => new GoodsList(x)));
        }


        [HttpGet("{id}")]
        public async Task<IActionResult> GetById(int id) {
            var item = await _database.Goods
                .Include(x => x.Ratings)
                .Include(x => x.Manufacturer)
                .FirstOrDefaultAsync(x => x.ID == id);
            if (item == null) {
                return new NotFoundResult();
            }

            item.Category = await _database.Categories.FindAsync(item.CategoryID);
            return new JsonResult(_mapper.Map<GoodsDetail>(item));
        }

        [HttpPost]
        public async Task<IActionResult> PostData(GoodsNew goods) {
            var item = _mapper.Map<Goods>(goods);
            await _database.Goods.AddAsync(item);
            await _database.SaveChangesAsync();
            return new CreatedResult("api/goods/" + item.ID, goods);
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> PutData(int id, GoodsUpdate goods) {
            var item = await _database.Goods.FindAsync(id);
            if (item == null) {
                return new NotFoundResult();
            }

            _database.Goods.Update(item);
            item.Update(_mapper.Map<Goods>(goods));
            await _database.SaveChangesAsync();
            return new JsonResult(item);
        }

        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteData(int id) {
            var item = await _database.Goods.FindAsync(id);
            if (item == null) {
                return new NotFoundResult();
            }

            _database.Goods.Remove(item);
            await _database.SaveChangesAsync();
            return new OkResult();
        }
    }
}