using System.Linq;
using System.Threading.Tasks;
using API.Data;
using API.Models.ManufacturerModels;
using AutoMapper;
using Microsoft.AspNetCore.Http.Features;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace API.Controllers {
    [ApiController]
    [Route("api/[controller]")]
    public class ManufacturerController {
        private readonly ApplicationDbContext _database;
        private readonly IMapper _mapper;

        public ManufacturerController(ApplicationDbContext database, IMapper mapper) {
            _database = database;
            _mapper = mapper;
        }

        [HttpGet]
        public IActionResult GetAll() {
            return new JsonResult(_database.Manufacturers.Select(x => new ManufacturerList(x)));
        }

        [HttpGet("{id}")]
        public async Task<IActionResult> GetDetail(int id) {
            //test if item exist, if yes, return item, else return error
            var item = await _database.Manufacturers.Include(x => x.Goods)
                .FirstOrDefaultAsync(x => x.ID == id);
            if (item == null) {
                return new NotFoundResult();
            }

            var res = _mapper.Map<ManufacturerDetail>(item);
            return new JsonResult(res);
        }

        [HttpPost]
        public async Task<IActionResult> PostData(ManufacturerNew man) {
            var item = _mapper.Map<Manufacturer>(man);
            await _database.Manufacturers.AddAsync(item);
            await _database.SaveChangesAsync();
            return new CreatedResult("api/manufacturer/" + item.ID, man);
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> PutData(ManufacturerUpdate man, int id) {
            //test if item exists
            var item = await _database.Manufacturers.FindAsync(id);
            if (item == null) {
                return new NotFoundResult();
            }

            _database.Manufacturers.Update(item);
            item.Update(_mapper.Map<Manufacturer>(man));
            await _database.SaveChangesAsync();
            return new JsonResult(item);
        }

        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteData(int id) {
            //test if item exists
            var item = await _database.Manufacturers.FindAsync(id);
            if (item == null) {
                return new NotFoundResult();
            }

            _database.Manufacturers.Remove(item);
            await _database.SaveChangesAsync();
            return new OkResult();
        }
    }
}